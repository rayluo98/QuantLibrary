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
        stock.updatePx(100);
        stock.updateQty(1);
        stockList.push_back(stock);
        Option op1(stockList, European, 100, 1, 0, 0.1, 5);
        auto BSmodel = BSModel(op1);
        BSmodel.CalculateBS(10000, 1000, true);
        std::cout << BSmodel._option.getPremium() << std::endl;

        return EXIT_SUCCESS;
};