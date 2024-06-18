#include "../pricers/option.hpp"
#include "model.hpp"
using namespace Derivatives;

class BSModel : public Model
{
public:
	Option _option;
	BSModel(Option option)
	{
		path_hist = vector<SamplePath>();
		_option = option;
	}
	void GenerateSamplePath(double T, int m, SamplePath& S);
	void Update_Params();
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
	double St = S[0];
	for (int k = 0; k < m; k++)
	{
		S[k] = St * exp((drift - sigma * sigma * 0.5) * (T / m) + sigma * sqrt(T / m) * Gauss());
		St = S[k];
	}
};
void BSModel::Update_Params(){
    // This is where we implement how to update the drift/sigm terms from prices
};

void BSModel::Update_Params(double mu, double sig){
    drift = mu;
    sigma = sig;
};