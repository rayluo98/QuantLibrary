#include"derivatives.hpp"

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
        explicit Option(const vector<Asset> _underlyers, optionStyle _style = European, double _k = 0, double _m = 0, double _r = 0,
            double _iv = 0, double _premium = 0) : style(_style), strike(_k), maturity(_m), r(_r), _implied_vol(_iv), _premium(_premium)
        {
            for (auto& i : _underlyers) {
                underlyers.push_back(i);
            }
        };
        Option(const Option& b) {
            underlyers = b.underlyers;
            strike = b.strike;
            maturity = b.maturity;
            r = b.r;
            _implied_vol = b._implied_vol;
            style = b.style;
            _premium = b._premium;
        }
        double strike;
        double maturity;
        double r;
        bool use_MC = true; // flag for calulcating premium using Monte Carlo ELSE PDE
        vector<Asset> underlyers;

        // this function loads premium -- to be overloaded if PDE solution is present
        double getPremium() {
            return _premium;
        }

        optionStyle getStyle() {
            return style;
        }

        void setPremium(double premium){
            _premium = premium;
        }

        double impliedVol() {
            // should be some kind of fitting logic
            return _implied_vol;
        };


    private:
        double _premium;
        double _implied_vol;
        optionStyle style;

    };

    
    class VanillaOption : public Option {
    public:
        explicit VanillaOption(vector<Asset> underlyer, double _k = 0, double _m = 0, double _r = 0) :
            Option(underlyer, European, _k, _m, _r) {};

    private:
        double Payoff(double S) { return max(0.0, S - strike); };
    };
    
}