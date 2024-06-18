#include"derivatives.hpp"


namespace Derivatives {
    enum optionStyle {
        European, American, Bermudan, Asian
    };

    enum priceProcess {
        BS, Bachelier, CGMY, LocalVol_Basic, StochVol_Basic
    };

    class Option : public Derivative {
    public:
        explicit Option(optionStyle _style = European, double _k = 0, double _m = 0, double _d = 0, double _r = 0, double _iv = 0) : 
            style(_style), strike(_k), maturity(_m), dividend_rate(_d), r(_r), _implied_vol(_iv){};
        Option(Option& b) {
            strike = b.strike;
            maturity = b.maturity;
            dividend_rate = b.dividend_rate;
            r = b.r;
            _implied_vol = b._implied_vol;
            style = b.style;
        }
        double strike;
        double maturity;
        double dividend_rate;
        double r;
        bool use_MC = true; // flag for calulcating premium using Monte Carlo ELSE PDE
        

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

        double impliedVol();


    private:
        double _premium;
        double _implied_vol;
        optionStyle style;

    };

    
    class VanillaOption : public Option {
    public:
        explicit VanillaOption(double _s = 0, double _m = 0, double _d = 0, double _r = 0) :
            Option(European, _s, _m, _d, _r) {};

    private:
        double Payoff(double S) { return max(0.0, S - strike); };
    };
    
}