#include "../assets/asset.h"
#include "model.hpp"

class Derivative : public Asset {
private:
	Asset underlying;
public:
	Derivative() : Asset() {};

	double Payoff(SamplePath& S);

	void Update();

	virtual double getPremium() = 0;
};