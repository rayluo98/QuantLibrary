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
    std::function<Number(Number)> myFunc;
    std::function<Number(Number)> myGrad;

    Solver(std::function<Number(Number)>_func, std::function<Number(Number)>_grad) {
        this->myFunc = _func;
        this->myGrad = _grad;
    }
    Solver(Number(*_func)(Number)) {
        myFunc = _func;
    }
    Solver(std::function<double(double)>_func, std::function<double(double)>_grad){
        myFunc = [_func](Number x) {return Number(_func(x.value())); };
        myGrad = [_grad](Number x) {return Number(_grad(x.value())); };
    }

    Solver(double(*_func)(double)){
        myFunc = [_func](Number x) {return Number(_func(x.value())); };
    }
    Solver(){}
};

// note that the issues with Newton's method is that it doesn't consider straddle points well

class NewtonMethod : Solver{
    // An example function whose solution is determined using
    // Bisection Method. The function is x^3 - x^2  + 2
public:
    bool hasGrad;

    NewtonMethod(std::function<Number(Number)>_func, std::function<Number(Number)>_grad):
        Solver(_func, _grad) {
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
        Number h = Number(x);
        Number::tape->rewind();
        auto guess = Number(x);
        if (hasGrad) {
            h = myFunc(guess) / myGrad(guess);
        }
        else { // use adjoints to calculate grad
            Number input = guess;
            Number y(myFunc(input));
            y.propagateToStart();
            
            if (abs(y.value()) < eps) {
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
                if (abs(y.value()) < eps or input.adjoint() == 0) {
                    break;
                }
                h = y.value() / input.adjoint();
            }
            guess = guess - h;
            cout << "Error: " << h.value() << endl;
        }

        return guess.value();
    }

};