<<<<<<<< HEAD:src/pricers/Models/BSM/BSModel.cpp
//include "option.h"
using namespace Model;
========
#include "../pricers/option.h"
#include "model.hpp"
using namespace Derivatives;

>>>>>>>> remotes/origin/dev:pricers/BSModel.h
class BSModel : public Model
{
public:
	Option _option;
	BSModel(Option option)
	{
		_option = option;
	}
	void GenerateSamplePath(double T, int m, SamplePath& S);
	void Update_Params();
	void Update_Params(double mu, double sig);
	void CalculateBS(unsigned int iteration = 10000, unsigned int mesh = 1){
		long sum = 0;
		double T = _option.maturity;
		for (int i = 0; i < iteration; ++i) {
			sum += 0;//_option.payGenerateSamplePath(T, mesh)
		}
		_option.setPremium(sum / iteration);
	}
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