#include"derivatives.hpp"
#include"../math_library/Solver/NewtonSolver.hpp"
/*
# The philosophy to this code base is as follows: we treat models as wrappers on top of contracts to reduce the load information storage
# Each derivatives contract is designed to have flexible contract details acts as a wrapper on top of some underlying asset(s)
# That's why for the code below, you may be surprised to find that the spot price and dividend rate are not stored directly. 
# These underlying price dynamics belong to the underlying and not the contract
*/
namespace Derivatives {
    enum optionStyle {
        European, American, Bermudan, Asian
    };

    enum priceProcess {
        BS, Bachelier, CGMY, LocalVol_Basic, StochVol_Basic
    };

    class Option : public Derivative {
    public:
        explicit Option(const vector<Asset> _underlyers, optionStyle _style = European, bool _isCall = true,
            double _k = 0, double _t = 0, double _m = 0, double _r = 0,
            double _iv = 0, double _premium = 0)
            : style(_style), isCall(_isCall), strike(_k), tenor(_t), maturity(_m), r(_r), _implied_vol(_iv), _premium(_premium)
        {
            for (auto& i : _underlyers) {
                underlyers.push_back(i);
            }
        };
        Option() :Derivative() {};
        Option(const Option& b) {
            underlyers = b.underlyers;
            strike = b.strike;
            maturity = b.maturity;
            r = b.r;
            _implied_vol = b._implied_vol;
            style = b.style;
            isCall = b.isCall;
        }
        double strike;
        double maturity;
        double tenor;
        double r;
        bool isCall;
        bool use_MC = true; // flag for calulcating premium using Monte Carlo ELSE PDEd
        virtual double Payoff(SamplePath& S) = 0;// { return max(0.0, (isCall ? 1 : -1) * (S.back() - strike)); };

        // this function loads premium -- to be overloaded if PDE solution is present
        double getPremium() {
            return this->Price();
        }

        optionStyle getStyle() {
            return style;
        }

        void setPremium(double premium){
            this->updatePx(premium);
        }

        double impliedVol() {
            // should be some kind of fitting logic
            return _implied_vol;
        };

        double _r() {
            return r;
        }

        virtual double PriceByBSFormula(double S0, double sigma, double r) { return 0.0; };
        // overload class as a function for PV to enable AAD
        // this CRPT to work as we need to override static function for AAD to work
        //static Number PVbyFormula(Number sigma) { return Number(); };

        void _fitIV(double px=0, bool useVega = false) {
            if (px != 0){
                setPremium(px);
            }
            // unfortunately cannot reference a member function as member function pointer is not convertible to functor in C
            auto PVbyFormula = [this](Number x) {
                auto S0 = underlyers.front().Price();
                auto d1 = log(S0 / strike) / x / sqrt(tenor) + (r + 0.5 * x * x) * tenor / x / sqrt(tenor);
                auto d2 = d1 - x * sqrt(tenor);
                
                //auto std = (x * sqrt(tenor));
                //return S0 * normalCdf(log(S0 / strike) / x / sqrt(tenor) + (r + 0.5 * pow(x, 2)) * tenor / x / sqrt(tenor))
                //    - strike * exp(-r * tenor) * normalCdf(log(S0 / strike) / x / sqrt(tenor) - (r + 0.5 * pow(x, 2)) * tenor / x / sqrt(tenor))
                // -_premium;
                
                return S0 * normalCdf(d1) - strike * exp(-r * tenor) * normalCdf(d2) - _premium;
            };

            auto VegaByFormula = [this](Number x) {
                auto S0 = underlyers.front().Price();
                //auto std = (x * sqrt(tenor));
                return normalDens(log(S0 / strike) / (x * sqrt(tenor)) + (r + 0.5 * x * x) * tenor / (x * sqrt(tenor))) * S0 * sqrt(tenor);
            };

            auto ivSolver = new NewtonMethod(PVbyFormula);
            // initial guess should be inflexion point of Call_BS(sigma)
            // I = sqrt(2 * log(m)/T), where m is moneyness = S/(K*exp(-r*tenor))
             double guess = sqrt(2 * log(abs(underlyers.front().Price() / strike / exp(-r * tenor)))/tenor);

            if (useVega) {
                auto ivSolver = new NewtonMethod(PVbyFormula, VegaByFormula);
                _implied_vol = ivSolver->newtonRaphson(guess, 1.0E-10);
            }
            else {
                auto ivSolver = new NewtonMethod(PVbyFormula);
                _implied_vol = ivSolver->newtonRaphson(guess, 1.0E-10);
            }
        };

        virtual void _fitIVS() {};

    private:
        double _premium;
        double _implied_vol;
        optionStyle style;

    };

    
    class EurOption : public Option {
    public:
        explicit EurOption(vector<Asset> underlyer, bool _isCall = true, double _k = 0, double _t = 0, double _m = 0, double _r = 0,
            double _iv = 0, double _premium = 0) :
            Option(underlyer, European, isCall, _k, _t, _m, _r, _iv, _premium) {};
        EurOption() :Option() {};

        double d_plus(double S0, double sigma, double r);
        double d_minus(double S0, double sigma, double r);
        double PriceByBSFormula(double S0, double sigma, double r);
        double VegaByBSFormula(double S0, double sigma, double r);
        double DeltaByBSFormula(double S0, double sigma, double r);

    private:
        double Payoff(SamplePath& S) { return max(0.0, (isCall ? 1 : -1) * (S.back() - strike)); };
    };
    
    double EurOption::d_plus(double S0, double sigma, double r)
    {
        return (log(S0 / strike) + (r + 0.5 * pow(sigma, 2.0)) * tenor) / (sigma * sqrt(tenor));
    }   

    double EurOption::d_minus(double S0, double sigma, double r)
    {
        return d_plus(S0, sigma, r) - sigma * sqrt(tenor);
    }
    double EurOption::PriceByBSFormula(double S0, double sigma, double r)
    {
        return S0 * N(d_plus(S0, sigma, r)) - strike * exp(-r * tenor) * N(d_minus(S0, sigma, r));
    }
    double EurOption::VegaByBSFormula(double S0, double sigma, double r)
    {
        return S0 * exp(-d_plus(S0, sigma, r) * d_plus(S0, sigma, r) / 2) * sqrt(tenor) / sqrt(2.0 * PI);
    }

    double EurOption::DeltaByBSFormula(double S0, double sigma, double r) {
        return N(d_plus(S0, sigma, r));
    }

    class DifferenceOfOptions : public Option
    {
    public:
        Option* Ptr1;
        Option* Ptr2;
        DifferenceOfOptions(double T_, double m_, Option* Ptr1_, Option* Ptr2_)
        {
            tenor = T_; maturity = m_; Ptr1 = Ptr1_; Ptr2 = Ptr2_;
            // we combine the underlyers from the two to options as our joint asset
            for (auto& i : Ptr1->underlyers) {
                underlyers.push_back(i);
            }
            for (auto& j : Ptr2->underlyers) {
                underlyers.push_back(j);
            }
        }
        double Payoff(SamplePath& S)
        {
            return Ptr1->Payoff(S) - Ptr2->Payoff(S);
        }
    };
}