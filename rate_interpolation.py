import numpy as np
import pandas as pd
from scipy.optimize import brentq

# ---------- Deposit handling ----------
def deposit_rate_to_df(rate, days, convention='ACT/360'):
    """Convert money-market rate to discount factor"""
    tau = days/360.0 if convention=='ACT/360' else days/365.0
    return 1.0 / (1.0 + rate * tau), tau

def bootstrap_deposits(deposits):
    times = [0.0]
    dfs   = [1.0]
    for d in deposits:
        D, tau = deposit_rate_to_df(d['rate'], d['tenor_days'])
        times.append(d['tenor_days']/360.0)
        dfs.append(D)
    return times, dfs

# ---------- Interpolation ----------
def interp_df(times, dfs, t):
    if t <= times[0]:
        return dfs[0]
    if t >= times[-1]:
        z = -np.log(dfs[-1])/times[-1]
        return np.exp(-z*t)
    for i in range(len(times)-1):
        if times[i] <= t <= times[i+1]:
            w = (t-times[i])/(times[i+1]-times[i])
            return dfs[i]**(1-w) * dfs[i+1]**w
    raise RuntimeError

# ---------- Futures convexity (Hull–White analytic) ----------
def hw_convexity_adjustment(t1,t2,a,sigma):
    """Analytic convexity adjustment for futures (Hull-White)"""
    # formula: CA = 0.5 * sigma^2/a^3 * (1 - e^{-a(t2-t1)})^2 * (1 - e^{-2at1})
    tau = t2-t1
    return 0.5 * sigma**2 / a**3 * (1 - np.exp(-a*tau))**2 * (1 - np.exp(-2*a*t1))

# ---------- Bootstrap main ----------
def bootstrap_curve(deposits,futures,swaps,a=0.1,sigma=0.01):
    times, dfs = bootstrap_deposits(deposits)

    convexity_list = []

    # --- Futures stage ---
    for fut in futures:
        t1, t2 = fut['start'], fut['end']
        tau = t2 - t1
        fut_rate = (100.0 - fut['fut_price'])/100.0

        ca = hw_convexity_adjustment(t1,t2,a,sigma)
        fwd_used = fut_rate + ca
        convexity_list.append({'interval':(t1,t2),'fut_rate':fut_rate,'ca':ca,'adj_forward':fwd_used})

        D1 = interp_df(times,dfs,t1)
        D2 = D1/(1+tau*fwd_used)
        times.append(t2); dfs.append(D2)

    # --- Swaps stage ---
    for tenor,par in swaps:
        pay_times = np.arange(1,tenor+1)
        for T in pay_times:
            if T in times: continue
            def f(Dguess):
                tmp_times = times+[T]
                tmp_dfs = dfs+[Dguess]
                sum_df = sum(interp_df(tmp_times,tmp_dfs,t) for t in range(1,T+1))
                D_T = interp_df(tmp_times,tmp_dfs,T)
                implied = (1-D_T)/sum_df
                return implied - par
            sol = brentq(f,1e-8,1.0)
            times.append(T); dfs.append(sol)

    # Sort
    idx = np.argsort(times)
    times = list(np.array(times)[idx])
    dfs   = list(np.array(dfs)[idx])

    return pd.DataFrame({'time':times,'df':dfs}), pd.DataFrame(convexity_list)

# ---------- Example ----------
deposits = [
    {'tenor_days':1,'rate':0.0004},
    {'tenor_days':7,'rate':0.00045},
    {'tenor_days':30,'rate':0.00055},
]

futures = [
    {'start':30/360,'end':0.5,'fut_price':99.70},
    {'start':0.5,'end':0.75,'fut_price':99.65},
    {'start':0.75,'end':1.0,'fut_price':99.60},
]

swaps = [
    (1,0.0035),
    (2,0.0070),
    (3,0.0105)
]

curve, convexity = bootstrap_curve(deposits,futures,swaps,a=0.1,sigma=0.01)

print("Bootstrapped curve:")
print(curve)
print("\nConvexity adjustments:")
print(convexity)
