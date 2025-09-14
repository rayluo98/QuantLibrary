"""
Ready-to-run Python notebook/script: Implied-Variance Surface PCA

What it does
1) Ingests a CSV of option IV observations (one row per option quote per timestamp).
2) Builds a consistent (log-moneyness k, tenor T) grid and interpolates each timestamp's IV surface onto that grid.
3) Converts IV -> implied variance (v = sigma^2) and optionally log-variance.
4) Computes Black-Scholes vega weights for each grid cell.
5) Runs PCA on: raw variance, log-variance, and vega-weighted variance.
6) Exports labeled components (loadings) and time series scores to CSV and NPZ.

Expectations for the input CSV (column names are case-insensitive):
- timestamp : observation timestamp (e.g. 2024-12-31 or 2024-12-31 15:45:00). This groups the surface snapshots.
- expiry    : expiration date of the option (YYYY-MM-DD) or exact datetime.
- strike    : strike price (float)
- mid_iv    : mid implied volatility in decimal (e.g. 0.23 for 23%). If you have sigma in percent, convert beforehand.
- underlying_price : (optional) the spot price S on that timestamp. If missing per-row, the code will take the median S for that timestamp.
- option_type : (optional) 'C' or 'P' (call/put). Not required for vega (vega same magnitude).

Notes
- The script interpolates implied **variance** (sigma^2) onto the grid. You can easily switch to sigma or log-variance.
- Use vega-weighting to get components that map to P&L by multiplying columns by sqrt(vega) before PCA.

Dependencies
pandas, numpy, scipy, sklearn

Usage
python implied_vol_pca_notebook.py --input my_iv_data.csv --outdir ./pca_out --grid_k "-0.6:0.3:21" --grid_T "7,30,90,180,365" --freq daily

"""

import os
import argparse
from datetime import datetime
import numpy as np
import pandas as pd
from math import log, sqrt, exp
from sklearn.decomposition import PCA
from scipy.interpolate import griddata

# -------------------------- Utilities ---------------------------------

def parse_args():
    p = argparse.ArgumentParser(description='Implied-Variance Surface PCA')
    p.add_argument('--input', required=True, help='CSV file with option IV observations')
    p.add_argument('--outdir', default='./pca_out', help='Output directory')
    p.add_argument('--grid_k', default='-0.6:0.3:21',
                   help='log-moneyness grid specification as start:end:count (e.g. -0.6:0.3:21)')
    p.add_argument('--grid_T', default='7,30,90,180,365',
                   help='comma-separated list of tenors in days (e.g. 7,30,90)')
    p.add_argument('--freq', default='daily', choices=['daily','intraday'],
                   help='How to group timestamps; daily groups by date only.')
    p.add_argument('--min_obs_per_surface', type=int, default=20,
                   help='Minimum number of observed option rows per timestamp to attempt interpolation')
    p.add_argument('--method', default='variance', choices=['variance','log-variance','sigma'],
                   help='Which transformed IV to run PCA on (variance recommended)')
    p.add_argument('--n_components', type=int, default=6, help='Number of PCA components to save')
    return p.parse_args()


def ensure_dir(d):
    if not os.path.exists(d):
        os.makedirs(d)

# Black-Scholes vega (annualized vol input). We'll use standard BS vega formula.
# vega = S * phi(d1) * sqrt(T) where phi is standard normal pdf.

def bs_vega(S, K, T, sigma):
    # handle edge cases
    if T <= 0 or sigma <= 0 or S <= 0 or K <= 0:
        return 0.0
    d1 = (np.log(S / K) + 0.5 * sigma * sigma * T) / (sigma * np.sqrt(T))
    phi = np.exp(-0.5 * d1 * d1) / np.sqrt(2.0 * np.pi)
    return S * phi * np.sqrt(T)

# time-to-expiry in years

def yearfrac(date_obs, date_exp):
    return (date_exp - date_obs).days / 365.0

# parse grid_k like '-0.6:0.3:21' into numpy array

def parse_k_grid(s):
    a, b, n = s.split(':')
    return np.linspace(float(a), float(b), int(n))

# parse grid_T like '7,30,90' into years

def parse_T_grid(s):
    days = [int(x.strip()) for x in s.split(',') if x.strip()]
    return np.array(days) / 365.0

# ---------------------------------------------------------------
# Main pipeline functions
# ---------------------------------------------------------------

def load_and_prepare(csv_path, freq='daily'):
    df = pd.read_csv(csv_path)
    # standardize column names
    df.columns = [c.strip().lower() for c in df.columns]
    required = ['timestamp','expiry','strike','mid_iv']
    for c in required:
        if c not in df.columns:
            raise ValueError(f'input CSV must contain column: {c}')

    # parse datetimes
    df['timestamp'] = pd.to_datetime(df['timestamp'])
    df['expiry'] = pd.to_datetime(df['expiry'])

    if 'underlying_price' in df.columns:
        df['underlying_price'] = pd.to_numeric(df['underlying_price'], errors='coerce')
    else:
        df['underlying_price'] = np.nan

    # group key: date or exact timestamp
    if freq == 'daily':
        df['obs_date'] = df['timestamp'].dt.date
    else:
        df['obs_date'] = df['timestamp']

    # clean values
    df['strike'] = pd.to_numeric(df['strike'], errors='coerce')
    df['mid_iv'] = pd.to_numeric(df['mid_iv'], errors='coerce')

    # Drop bad rows
    df = df.dropna(subset=['strike','mid_iv','expiry'])

    # ensure IV is decimal (if >2, assume percent and divide)
    iv_median = df['mid_iv'].median()
    if iv_median > 2.0:
        df['mid_iv'] = df['mid_iv'] / 100.0

    return df


def build_grid_and_interpolate(df, ks, Ts, min_obs_per_surface=20, method='variance'):
    # ks: array of log-moneyness
    # Ts: array of tenors in years
    # Returns: surfaces (n_obs x n_features), index (obs_dates), feature_coords DataFrame with columns k, T, K_ref (for reference)

    feature_coords = []
    for T in Ts:
        for k in ks:
            feature_coords.append({'k': float(k), 'T': float(T)})
    feat_df = pd.DataFrame(feature_coords)
    n_features = len(feat_df)

    obs_keys = sorted(df['obs_date'].unique())
    surfaces = []
    obs_list = []

    for obs in obs_keys:
        sub = df[df['obs_date'] == obs].copy()
        if len(sub) < min_obs_per_surface:
            # skip surfaces with too few points
            continue

        # determine spot per obs: prefer explicit underlying_price; else median of strike-adj implied forward? use median underlying_price if present
        S_val = sub['underlying_price'].median()
        if np.isnan(S_val):
            # fallback: pick max strike near ATM? We'll use median strike as crude S if underlying missing
            S_val = sub['strike'].median()

        # compute k and T for each quote
        sub['T'] = sub['expiry'].apply(lambda e: yearfrac(pd.to_datetime(obs), pd.to_datetime(e)))
        sub = sub[sub['T'] > 0.0].copy()
        if len(sub) < min_obs_per_surface:
            continue

        sub['k'] = np.log(sub['strike'] / S_val)
        # convert to variance
        sub['variance'] = sub['mid_iv'] ** 2

        # prepare points and values for interpolation: use (k, T)
        points = np.vstack([sub['k'].values, sub['T'].values]).T
        values = sub['variance'].values

        # feature points
        grid_points = np.vstack([feat_df['k'].values, feat_df['T'].values]).T

        # griddata interpolation: linear with nearest for NaNs
        try:
            grid_v = griddata(points, values, grid_points, method='linear')
        except Exception:
            grid_v = np.full((n_features,), np.nan)

        # fill NaNs with nearest
        nanmask = np.isnan(grid_v)
        if nanmask.any():
            try:
                near = griddata(points, values, grid_points[nanmask], method='nearest')
                grid_v[nanmask] = near
            except Exception:
                pass

        # still NaNs -> skip this surface
        if np.isnan(grid_v).any():
            # as last resort, fill with median variance
            med = np.nanmedian(values)
            grid_v = np.where(np.isnan(grid_v), med, grid_v)

        # apply method transformation
        if method == 'variance':
            arr = grid_v
        elif method == 'log-variance':
            # Avoid log of zero
            arr = np.log(np.clip(grid_v, 1e-9, None))
        elif method == 'sigma':
            arr = np.sqrt(grid_v)
        else:
            arr = grid_v

        surfaces.append(arr)
        obs_list.append(obs)

    if len(surfaces) == 0:
        raise ValueError('No valid surfaces constructed. Check min_obs_per_surface or the input data grouping.')

    surfaces = np.vstack(surfaces)  # shape (n_obs, n_features)
    return surfaces, obs_list, feat_df


def compute_vega_weights(feat_df, surfaces, obs_list, df_original):
    # Compute representative vega per feature by using average sigma on that grid cell across time
    # We'll compute average sigma per feature from surfaces (if surfaces are variance, take sqrt)
    # Need a mapping of obs_date->representative S used when building surfaces. We'll recompute S per obs_date from df_original median.

    # compute mean sigma per feature
    mean_var = np.mean(surfaces, axis=0)
    mean_sigma = np.sqrt(np.clip(mean_var, 1e-9, None))

    # for S by obs: take median underlying_price per obs_list
    S_map = {}
    for obs in obs_list:
        sub = df_original[df_original['obs_date'] == obs]
        S_val = sub['underlying_price'].median()
        if np.isnan(S_val):
            S_val = sub['strike'].median()
        S_map[obs] = float(S_val)

    # choose a representative S: median of S_map values
    S_rep = np.median(list(S_map.values()))

    # feature T and k
    ks = feat_df['k'].values
    Ts = feat_df['T'].values

    vegas = np.array([bs_vega(S_rep, np.exp(k) * S_rep, T, sigma)
                      for k, T, sigma in zip(ks, Ts, mean_sigma)])

    # normalize weights (so mean weight = 1)
    w = vegas.copy().astype(float)
    # avoid zero
    w[w <= 0] = np.nan
    meanw = np.nanmean(w)
    if np.isnan(meanw) or meanw == 0:
        w = np.ones_like(w)
    else:
        w = w / meanw

    # replace NaNs with 1
    w = np.where(np.isnan(w), 1.0, w)
    return w


def run_pca_and_save(surfaces, feat_df, obs_list, weights=None, n_components=6, outdir='./pca_out', prefix='variance'):
    # surfaces: (n_obs, n_features)
    # weights: (n_features,) multiplicative on columns (e.g., sqrt(vega) or vega)

    # demean columns
    X = surfaces.copy().astype(float)
    X_mean = X.mean(axis=0)
    Xc = X - X_mean[np.newaxis, :]

    if weights is not None:
        Wsqrt = np.sqrt(weights)
        Xc_weighted = Xc * Wsqrt[np.newaxis, :]
    else:
        Xc_weighted = Xc

    pca = PCA(n_components=n_components)
    scores = pca.fit_transform(Xc_weighted)
    comps = pca.components_  # shape (n_components, n_features)
    explained = pca.explained_variance_ratio_

    # if weights were applied, adjust components to original (unweighted) feature space for interpretation
    if weights is not None:
        # comps_weighted * sqrt(W) = comps_unweighted  => comps_unweighted = comps_weighted * sqrt(W)
        comps_unweighted = comps * np.sqrt(weights)[np.newaxis, :]
    else:
        comps_unweighted = comps

    # save loadings (components) as CSV with feature coordinates
    comp_df_list = []
    for i in range(comps_unweighted.shape[0]):
        row = {
            'component': f'PC{i+1}',
            'explained_variance_ratio': float(explained[i])
        }
        comp_df_list.append(row)

    comps_save = pd.DataFrame(comps_unweighted, columns=[f'feat_{j}' for j in range(comps_unweighted.shape[1])])
    comps_save.insert(0, 'component', [f'PC{i+1}' for i in range(comps_unweighted.shape[0])])

    # feature coordinate file
    feat_coords = feat_df.copy().reset_index(drop=True)
    feat_coords['feat_idx'] = ['feat_%d' % i for i in range(len(feat_df))]

    # wide components saved with feature names matched to coords
    comps_wide = comps_save.copy()
    comps_wide.columns = ['component'] + list(feat_coords['feat_idx'])

    # Save files
    ensure_dir(outdir)
    comps_wide.to_csv(os.path.join(outdir, f'{prefix}_components_wide.csv'), index=False)
    feat_coords.to_csv(os.path.join(outdir, f'{prefix}_feature_coords.csv'), index=False)

    scores_df = pd.DataFrame(scores, columns=[f'PC{i+1}_score' for i in range(scores.shape[1])])
    scores_df.insert(0, 'obs_date', obs_list)
    scores_df.to_csv(os.path.join(outdir, f'{prefix}_scores.csv'), index=False)

    # also save numpy archive with arrays
    np.savez(os.path.join(outdir, f'{prefix}_pca.npz'), comps=comps_unweighted, scores=scores, mean=X_mean, explained=explained)

    summary = pd.DataFrame({'component': [f'PC{i+1}' for i in range(len(explained))], 'explained_variance_ratio': explained})
    summary.to_csv(os.path.join(outdir, f'{prefix}_explained.csv'), index=False)

    print(f'Saved PCA artifacts to {outdir} with prefix {prefix}')
    return {
        'components': comps_unweighted,
        'scores': scores,
        'mean': X_mean,
        'explained': explained
    }

# ---------------------------------------------------------------
# Command-line main
# ---------------------------------------------------------------
if __name__ == '__main__':
    args = parse_args()
    ensure_dir(args.outdir)

    print('Loading data...')
    df = load_and_prepare(args.input, freq=args.freq)

    ks = parse_k_grid(args.grid_k)
    Ts = parse_T_grid(args.grid_T)

    print('Building and interpolating surfaces...')
    surfaces, obs_list, feat_df = build_grid_and_interpolate(df, ks, Ts,
                                                            min_obs_per_surface=args.min_obs_per_surface,
                                                            method='variance')

    # Save a preview of surfaces
    np.savez(os.path.join(args.outdir, 'surfaces_raw.npz'), surfaces=surfaces, obs_list=obs_list)

    print('Computing vega weights...')
    vega_weights = compute_vega_weights(feat_df, surfaces, obs_list, df)
    # Save feature and vega info
    feat_df_copy = feat_df.copy()
    feat_df_copy['vega_weight'] = vega_weights
    feat_df_copy.to_csv(os.path.join(args.outdir, 'feature_coords_with_vega.csv'), index=False)

    print('Running PCA on raw variance...')
    p_raw = run_pca_and_save(surfaces, feat_df, obs_list, weights=None,
                             n_components=args.n_components, outdir=args.outdir, prefix='variance_raw')

    print('Running PCA on log-variance...')
    # build log-variance surfaces
    log_surfaces = np.log(np.clip(surfaces, 1e-9, None))
    p_log = run_pca_and_save(log_surfaces, feat_df, obs_list, weights=None,
                             n_components=args.n_components, outdir=args.outdir, prefix='variance_log')

    print('Running PCA on vega-weighted variance...')
    p_vega = run_pca_and_save(surfaces, feat_df, obs_list, weights=vega_weights,
                              n_components=args.n_components, outdir=args.outdir, prefix='variance_vega_weighted')

    print('\nAll done. Outputs saved into', args.outdir)
    print('Files include: variance_raw_components_wide.csv, variance_raw_scores.csv, variance_log_components_wide.csv, etc.')

    print('\nSuggestions:')
    print('- Inspect feature_coords_with_vega.csv to ensure grid and vega weights look sensible.')
    print('- Visualize components by reshaping component vectors into (len(Ts), len(ks)).')
    print('- Try PCA on Δvariance to focus on dynamics rather than levels.')

