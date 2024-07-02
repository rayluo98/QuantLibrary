#include "../assets/asset.h"
#include "model.hpp"

class Derivative : public Asset {
public:
	vector<Asset> underlyers;
	Derivative() : Asset() {};

	double Payoff(SamplePath& S);

	void Update();

	virtual double getPremium() = 0;
};