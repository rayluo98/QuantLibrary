#pragma once
#include"NewtonSolver.hpp"
#include<assert.h>

Number f(Number x) {
	return x * x - 100;
}

Number g(Number x) {
	return cos(x) - x * x * x;
}

namespace SolverTest {
	int main(int argc, char** argv)
	{
		auto solvTest = new NewtonMethod(f);
		assert(abs(solvTest->newtonRaphson(100) - 10) < EPSILON);

		solvTest = new NewtonMethod(g);
		assert(abs(solvTest->newtonRaphson(10) - 0.865474) < EPSILON);
		return 0;
	};
}