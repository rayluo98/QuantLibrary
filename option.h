#include "asset.h"

enum optionStyle {
    European, American, Bermudan, Asian
};

enum priceProcess{
    BS, Bachelier, CGMY, LocalVol_Basic, StochVol_Basic
};

class Option : public Derivative {
    public:
        double strike;
        double maturity;
        double dividend_rate; 
        double r; 

        double premium()

        optionStyle getStyle() {
            return style;
        }

        double impliedVol()


    private:
        double premium;
        double implied_vol; 
        optionStyle style;
}

class VanillaOption : public Option { 
    public: 

}