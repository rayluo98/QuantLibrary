#include <ctime>
#include <string>
#include <iostream>
#include "main.h"
#include "assets/option.h"
#include "pricers/BSModel.h"
#include "math_library/matrix.h"

using namespace QLabs;

int QLabs::_run(int argc, char** argv) {
        std::string who = "world";
        Option option(European, 100, 11, 0, 0);
        std::cout << option.getPremium() << std::endl;

        return EXIT_SUCCESS;
};