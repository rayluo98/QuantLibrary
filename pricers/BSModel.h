#include "../pricers/option.hpp"
#include "model.hpp"
#include"../market/market.hpp"
#pragma once
using namespace Derivatives;

class BSModel : public Model
{
public:
	Option _option;
	BSModel(Option option):_option(option)
	{
		path_hist = vector<SamplePath>();
		Update_Params();
	}
	BSModel(const BSModel& mod1) : _option(mod1._option), path_hist(mod1.path_hist) { Update_Params(); };

	void GenerateSamplePath(double T, int m, SamplePath& S);
	void Update_Params();
	void Update_Params(Referential hist);
	void Update_Params(double mu, double sig);
	void CalculateBS(unsigned int iteration = 10000, unsigned int mesh = 1, bool storeSamples = false){
		long sum = 0;
		// double T = _option.maturity;
		for (unsigned int i = 0; i < iteration; ++i) {
			SamplePath path;
			GenerateSamplePath(_option.maturity, mesh, path);
			sum += path[mesh];
			if (storeSamples)
				path_hist.push_back(path);
		}
		_option.setPremium(sum / iteration);
	}
private:
	vector<SamplePath> path_hist;
};

void BSModel::GenerateSamplePath(double T, int m, SamplePath& S)
{
	S.resize(m);
	S[0] = _option.underlyers[0].Price();
	double St = S[0];
	for (int k = 0; k < m; k++)
	{
		S[k] = St * exp((drift - sigma * sigma * 0.5) * (T / m) + sigma * sqrt(T / m) * Gauss());
		St = S[k];
	}
};
void BSModel::Update_Params(Referential hist){
    // This is where we implement how to update the drift/sigm terms from prices
	return;
};

void BSModel::Update_Params() {
	// Default update taking forward rate as drift and vol as implied vol
	drift = _option.r;
	sigma = _option.impliedVol();
}

void BSModel::Update_Params(double mu, double sig){
    drift = mu;
    sigma = sig;
};