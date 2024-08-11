// C++ program for implementation of Newton Raphson Method for
// solving equations
#pragma once
//#include<bits/stdc++.h>
#include<functional>
#include"../AAD/AAD.cpp"
#include <iostream>

using namespace std;
typedef Number (*func)(Number);
typedef Number(*grad)(Number); 
typedef double(*func_dbl)(double);
typedef double(*grad_dbl)(double);

#define EPSILON 0.0001

class Solver {

public:
    func myFunc;
    grad myGrad;

    Solver(Number(*_func)(Number), Number(*_grad)(Number)) {
        this->myFunc = _func;
        this->myGrad = _grad;
    }
    Solver(Number(*_func)(Number)) {
        myFunc = _func;
    }
    Solver(double(*_func)(double)) {
        myFunc = reinterpret_cast<func>(_func);
    }
    Solver(double(*_func)(double), double(*_grad)(double)) {
        myFunc = reinterpret_cast<func>(_func);
        myGrad = reinterpret_cast<grad>(_grad);
    }
    Solver(){}
};

// note that the issues with Newton's method is that it doesn't consider straddle points well

class NewtonMethod : Solver{
    // An example function whose solution is determined using
    // Bisection Method. The function is x^3 - x^2  + 2
public:
    bool hasGrad;

    NewtonMethod(Number(*_func)(Number), Number(*_grad)(Number)):
        Solver(*_func, *_grad) {
        hasGrad = true;
    }

    NewtonMethod(Number(*_func)(Number)) : Solver(*_func){
        hasGrad = false;
    }

    NewtonMethod(double(*_func)(double), double(*_grad)(double)) : Solver(*_func, *_grad) {
        hasGrad = true;
    }

    NewtonMethod(double(*_func)(double)) : Solver(*_func){
        hasGrad = false;
    }

    NewtonMethod() {}

    // Function to find the root
    double newtonRaphson(double x, double eps = EPSILON)
    {
        Number h;
        Number::tape->rewind();
        auto guess = Number(x);
        if (hasGrad) {
            h = myFunc(guess) / myGrad(guess);
        }
        else { // use adjoints to calculate grad
            Number input = guess;
            Number y(myFunc(input));
            if (input.adjoint()) {
                throw exception("Gradient is zero! Straddle alert!");
            }
            else if (y == 0) {
                return x;
            }
            h = y / input.adjoint();
        }
        while (abs(h.value()) >= eps)
        {
            if (hasGrad) {
                h = myFunc(guess) / myGrad(guess);
            }
            else { // use adjoints to calculate grad
                Number input = guess;
                Number y(myFunc(guess));
                y.propagateToStart();
                if (input.adjoint()) {
                    throw exception("Gradient is zero! Straddle alert!");
                }
                else if (y == 0) {
                    return y.value();
                }
                h = y.value() / input.adjoint();
            }
            guess = guess - h;
            cout << "Error: " << h.value() << endl;
        }

        return guess.value();
    }

};