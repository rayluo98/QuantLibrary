#pragma once
#include <cmath>
#include <vector>
#include "cubic.cc"

namespace QMath {
	static class QMath {
	public:
		// fits cubic spline curve to set of 3 points
		// returns coefficients to cubic
		// input are set of n points that define the cubic
		static std::vector<double*> cublicSpline(std::vector<std::pair<double, double>> inputs){}
    };
}