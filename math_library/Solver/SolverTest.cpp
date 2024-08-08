#pragma once
#include"NewtonSolver.hpp"
#include<assert.h>

Number f(Number x) {
	return x * x - 100;
}

namespace SolverTest {
	int main(int argc, char** argv)
	{
		auto solvTest = new NewtonMethod(f);
		assert(abs(solvTest->newtonRaphson(100) - 10) < EPSILON);
		return 0;
	};
}