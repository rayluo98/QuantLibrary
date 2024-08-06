// C++ program for implementation of Newton Raphson Method for
// solving equations
#pragma once
//#include<bits/stdc++.h>
#include<functional>
#include"../AAD/AAD.h"
#include <SolverTest.cpp>
#include <iostream>

using namespace std;


namespace SolverLib {

    template<class T>
    class Solver {
    public:
        function<(T)> func;
        function<T(T)> grad;
        Solver(T(*_func)(T), T(*_grad)(T)) {
            func = _func;
            grad = _grad;
        }
        Solver(){}
    };


    template<class T>
    class NewtonMethod : Solver<T> {
        // An example function whose solution is determined using
        // Bisection Method. The function is x^3 - x^2  + 2
    public:
        bool hasGrad;

        NewtonMethod(T (*_func)(T), T (*_grad)(T)):
            Solver(_func, _grad) {
            hasGrad = true;
        }

        NewtonMethod(T (*_func)(T)) {
            hasGrad = false;
        }

        // Function to find the root
        double newtonRaphson(double x, double EPSILON = 0.0001)
        {
            double h;
            Number::tape->rewind();
            if (hasGrad) {
                h = func(x) / grad(x);
            }
            else { // use adjoints to calculate grad
                Number input[1] = { x };
                Number y = func(input);
                y.propagateAdjoints();
                h = y.value() / input[0].adjoint();
            }
            while (abs(h) >= EPSILON)
            {
                if (hasGrad) {
                    h = func(x) / derivFunc(x);
                }
                else { // use adjoints to calculate grad
                    Number input[1] = { x };
                    Number y = func(input);
                    y.propagateAdjoints();
                    h = y.value() / input[0].adjoint();
                }
                x = x - h;
            }

            return x;
        }

    };
    
    /*
    // Driver program to test above
    int main()
    {
        double x0 = -20; // Initial values assumed
        cout << "The value of the root is : " << SolverLib::NewtonMethod<Number>::newtonRaphson(x0);
        return 0;
    }*/
}