#pragma once
#include"NewtonSolver.hpp"
#include<assert.h>

Number f(Number x) {
	return x * x - 100;
}

Number g(Number x) {
	return cos(x) - x * x * x;
}

Number h(Number x) {
	return x*pow((1+pow(x,2)),-1/2);
}

namespace SolverTest {
	int main(int argc, char** argv)
	{
		auto solvTest = new NewtonMethod(f);
		assert(abs(solvTest->newtonRaphson(100) - 10) < EPSILON);

		solvTest = new NewtonMethod(g);
		assert(abs(solvTest->newtonRaphson(10) - 0.865474) < EPSILON);

		solvTest = new NewtonMethod(h);
		//assert(abs(solvTest->newtonRaphson(10) - 0) > EPSILON); // this is known not to converge actually
		//assert(abs(solvTest->newtonRaphson(1) - 0) > EPSILON); // this is known to oscillate
		assert(abs(solvTest->newtonRaphson(0.5) - 0) > EPSILON); // converges
		return 0;
	};
}