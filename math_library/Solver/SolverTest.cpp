#pragma once
#include"NewtonSolver.hpp"
#include<assert.h>

using namespace SolverLib;
inline double f(double x) {
	return x * x;
}

namespace SolverTest {
	int main(int argc, char** argv)
	{
		auto solvTest = new SolverLib::NewtonMethod<double>(f);
		return 0;
	};
}