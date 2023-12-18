#include "assets/asset.h"
#include "model.hpp"

class Derivative : public Asset {
private:
	Asset underlying;
public:
	Derivative() : Asset() {};

	void Update();

	virtual double getPremium() = 0;
};