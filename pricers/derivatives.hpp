#include "assets/asset.h"

class Derivative {
public:
	Asset underlying;

	double getPremium(Model mod) = 0;
};