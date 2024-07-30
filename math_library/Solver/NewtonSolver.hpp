// C++ program for implementation of Newton Raphson Method for
// solving equations
#include<bits/stdc++.h>
#include<functional>
#include"../AAD/AAD.cpp"
using namespace std;


namespace SolverLib {

    class Solver {
        function<double(double)> func;
        function<double(double)> grad;
        Solver(double (*_func)(double), double (*_grad)(double)) {
            func = _func;
            grad = _grad;
        }
    };

    class NewtonMethod :Solver {
        // An example function whose solution is determined using
        // Bisection Method. The function is x^3 - x^2  + 2
    public:
        bool hasGrad;

        NewtonMethod(double (*_func)(double), double (*_grad)(double)):
            Solver(_func, _grad) {
            hasGrad = true;
        }

        NewtonMethod(double (*_func)(double)) {
            hasGrad = false;
        }

        // Function to find the root
        double newtonRaphson(double x, double EPSILON = 0.0001)
        {
            double h;
            if (hasGrad) {
                h = func(x) / derivFunc(x);
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

    // Driver program to test above
    int main()
    {
        double x0 = -20; // Initial values assumed
        cout << "The value of the root is : " << newtonRaphson(x0);
        return 0;
    }
}