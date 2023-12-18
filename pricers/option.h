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
        Option() : Derivative() {};
        double strike;
        double maturity;
        double dividend_rate;
        double r;
        bool use_MC = true; // flag for calulcating premium using Monte Carlo ELSE PDE
        

        // this function loads premium -- to be overloaded if PDE solution is present
        double getPremium() {
            return premium;
        }

        optionStyle getStyle() {
            return style;
        }

        double impliedVol();


    private:
        double premium;
        double implied_vol;
        optionStyle style;
    };

    /*
    class VanillaOption : public Option {
        public:

    }
    */
}