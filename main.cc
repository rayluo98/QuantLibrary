#include <ctime>
#include <string>
#include <iostream>
#include "main.h"
#include "assets/option.h"
#include "pricers/BSModel.h"
#include "math_library/matrix.h"
#include <vector>

using namespace QLabs;

int QLabs::_run(int argc, char** argv) {
        std::string who = "world";
        Asset stock = Asset();
        vector<Asset> stockList;
        stock.updatePx(226.51);
        stock.updateQty(1);
        //stock.updateDiv(0.0044);
        stockList.push_back(stock);
        EurOption op1(stockList, true, 225, 30/360.0, 30/360.0, 0.0533, 0.3, 8.65);
        op1._fitIV(8.65);
        auto BSmodel = BSModel(&op1);
        BSmodel.Update_Params();
        BSmodel.CalculateMC(10000, 1000, 0.00001, true);
        std::cout << "MC Premium: " << op1.getPremium() << std::endl;
        std::cout << "BS Price: " << op1.PriceByBSFormula(stock.Price(), 0.2826, 0.0533) << std::endl;
        //EurOption op2(stockList, true, 100, 1.0, 1.0, 0, 0.1);
        EurOption op2(stockList, true, 225, 30 / 360.0, 30 / 360.0, 0.0533, 0.2826, 8.65);
        BSmodel.CalcVarReducMC(op2, 10000, 1000, 0.00001);
        std::cout << "VR Price: " << op1.getPremium() << std::endl;
        //std::cout << "Delta: " << op1.DeltaByBSFormula() << std::endl;

        return EXIT_SUCCESS;
};