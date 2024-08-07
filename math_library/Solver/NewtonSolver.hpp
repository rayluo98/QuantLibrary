// C++ program for implementation of Newton Raphson Method for
// solving equations
#pragma once
//#include<bits/stdc++.h>
#include<functional>
#include"../AAD/AAD.h"
#include <iostream>

using namespace std;
typedef double (*func)(double);
typedef double (*grad)(double);

namespace SolverLib {

    class Solver {


        func myFunc;
        grad myGrad;

    public:

        Solver(double (*_func)(double), double(*_grad)(double)) {
            this->myFunc = _func;
            this->myGrad = _grad;
        }
        Solver(double(*_func)(double)) {
            myFunc = _func;
        }
        Solver(){}
    };

    template<typename T>
    class NewtonMethod : Solver{
        // An example function whose solution is determined using
        // Bisection Method. The function is x^3 - x^2  + 2
    public:
        bool hasGrad;

        NewtonMethod(T (*_func)(T), T (*_grad)(T)):
            Solver(*_func, *_grad) {
            hasGrad = true;
        }

        NewtonMethod(T(*_func)(T)) : Solver(*_func){
            hasGrad = false;
        }

        NewtonMethod() {}

        // Function to find the root
        double newtonRaphson(double x, double EPSILON = 0.0001)
        {
            double h;
            Number::tape->rewind();
            auto guess = Number(x);
            if (hasGrad) {
                h = myFunc(x) / myGrad(x);
            }
            else { // use adjoints to calculate grad
                Number input[1] = { x };
                Number y = myFunc(input);
                y.propagateAdjoints();
                h = y.value() / input[0].adjoint();
            }
            while (abs(h) >= EPSILON)
            {
                if (hasGrad) {
                    h = myFunc(x) / myGrad(x);
                }
                else { // use adjoints to calculate grad
                    Number input[1] = { x };
                    Number y = myFunc(input);
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