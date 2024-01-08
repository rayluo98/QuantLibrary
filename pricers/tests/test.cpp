#include "test.h"
#include "..\BSModel.h"

#include <cassert>
#include <iostream>

namespace Derivative {
	void test() {
		// to fill with basic class tests
	}

	void test_math() {
		// we test the Gauss function with law of large numbers 
		auto samples = new std::vector<double>();
		for (int i = 0; i < 1000; ++i) {
			samples.push(Gauss());
		}
	}
}