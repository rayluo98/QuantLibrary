#include <cstdlib>
#include <ctime>
#include <cmath>
#include "model.h"

#pragma once

const double PI = atan(1) * 4;

double Gauss()
{
	double U1 = (rand() + 1.0) / (RAND_MAX + 1.0);
	double U2 = (rand() + 1.0) / (RAND_MAX + 1.0);
	return sqrt(-2.0 * log(U1)) * cos(2.0 * PI * U2);
}

/*
algorithm Poisson generator based upon the inversion by sequential search:
    init:
        Let x ← 0, p ← e^−λ, s ← p.
        Generate uniform random number u in [0,1].
    while u > s do:
        x ← x + 1.
        p ← p × λ / x.
        s ← s + p.
    return x.
*/
double Poisson(double lambda){
    double u = (rand() + 1.0) / (RAND_MAX + 1.0);
    double x = 0;
    double p = exp(-lambda);
    double s = p;
    while(u > s) {
        x++;
        p *= (lambda/x);
        s += p;
    }
    return x;
}

double N(double x)
{
    double gamma = 0.2316419; double a1 = 0.319381530;
    double a2 = -0.356563782; double a3 = 1.781477937;
    double a4 = -1.821255978; double a5 = 1.330274429;
    //double pi = 4.0 * atan(1.0);
    double k = 1.0 / (1.0 + gamma * x);
    if (x >= 0.0)
    {
        return 1.0 - ((((a5 * k + a4) * k + a3) * k + a2) * k + a1) * k * exp(-x * x / 2.0) / sqrt(2.0 * PI);
    }
    else return 1.0 - N(-x);
}