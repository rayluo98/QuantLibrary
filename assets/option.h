#include "asset.h"

namespace stdAsset {
    enum optionStyle {
        European, American, Bermudan, Asian
    };

    enum priceProcess {
        BS, Bachelier, CGMY, LocalVol_Basic, StochVol_Basic
    };

    // Note that this is the referential object and not the pricing object
    class Option : public Asset {
    public:
        Option() {
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
