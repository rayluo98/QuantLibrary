typedef std::vector<double> SamplePath;
#include <cstdlib>
#include <ctime>
using namespace std;

namespace Model{
    class Model{
        public:
            double drift, sigma;
            virtual void GenerateSamplePath(double T, int m, SamplePath& S)=0;
        private: 
            virtual void Update_Params()=0;
    }

    double Gauss()
    {
        double U1 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double U2 = (rand() + 1.0) / (RAND_MAX + 1.0);
        return sqrt(-2.0 * log(U1)) * cos(2.0 * pi * U2);
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
}