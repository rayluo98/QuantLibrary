// C++ program for implementation of Newton Raphson Method for
// solving equations
#include<bits/stdc++.h>
using namespace std;


namespace SolverLib {
    // An example function whose solution is determined using
    // Bisection Method. The function is x^3 - x^2  + 2
    double func(double x)
    {
        return x * x * x - x * x + 2;
    }

    // Derivative of the above function which is 3*x^x - 2*x
    double derivFunc(double x)
    {
        return 3 * x * x - 2 * x;
    }

    // Function to find the root
    double newtonRaphson(double x, double EPSILON = 0.0001)
    {
        double h = func(x) / derivFunc(x);
        while (abs(h) >= EPSILON)
        {
            h = func(x) / derivFunc(x);

            // x(i+1) = x(i) - f(x) / f'(x)  
            x = x - h;
        }

        return x;
    }

    // Driver program to test above
    int main()
    {
        double x0 = -20; // Initial values assumed
        cout << "The value of the root is : " << newtonRaphson(x0);
        return 0;
    }
}