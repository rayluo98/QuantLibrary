#include "asset.h"

namespace Asset {
    enum optionStyle {
        European, American, Bermudan, Asian
    };

    enum priceProcess {
        BS, Bachelier, CGMY, LocalVol_Basic, StochVol_Basic
    };

    class Option : public Asset {
    public:
        Option() {
            premium = 10.0;
        }

        double getPremium() {
            return premium;
        }

        optionStyle getStyle() {
            return style;
        }

        double getImpliedVol() {
            return implied_vol;
        }

        double getStrike() {
            return strike;
        }

        void Update() {
            // Do nothing
        }

    private:
        double premium;
        double implied_vol;
        optionStyle style;
        double strike;
        double maturity;
        double dividend_rate;
        double r;

    };
}
