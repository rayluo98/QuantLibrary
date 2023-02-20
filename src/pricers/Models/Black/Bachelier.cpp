
class BSModel : public Model
{
public:
	Option _option;
	BSModel(Option option)
	{
		_option = option;
	}
	void GenerateSamplePath(double T, int m, SamplePath& S);
};

const double pi = 4.0 * atan(1.0);

void BSModel::GenerateSamplePath(double T, int m, SamplePath& S)
{
	double St = S0;
	for (int k = 0; k < m; k++)
	{
		S[k] = St * exp((drift - sigma * sigma * 0.5) * (T / m) + sigma * sqrt(T / m) * Gauss());
		St = S[k];
	}
};
void BSModel::Update_Params(){
    // This is where we implement how to update the drift/sigm terms from prices
};

void BSModel::Update_Params(double mu, double sig){
    drift = mu;
    sigma = sig;
};