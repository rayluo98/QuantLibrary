#include"NewtonSolver.hpp"
#include<assert.h>

inline double square(double x) {
	return x * x;
}

int main(int argc, char** argv)
{
	auto solvTest = new SolverLib::Solver<Number>(square);
};