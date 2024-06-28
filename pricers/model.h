#include <vector>
using namespace std;
typedef std::vector<double> SamplePath;

class Model {
public:
    double drift, sigma;
    double PricingError, delta;
    virtual void GenerateSamplePath(double T, int m, SamplePath& S) = 0;
private:
    virtual void Update_Params() = 0;
};
