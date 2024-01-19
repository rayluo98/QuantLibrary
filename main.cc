#include <ctime>
#include <string>
#include <iostream>
#include "assets/option.h"
#include "pricers/BSModel.h"
#include "math_library/matrix.h"


namespace QLabs {
    int main(int argc, char** argv) {
        std::string who = "world";
        Option option;
        std::cout << option.getPremium() << std::endl;

        return EXIT_SUCCESS;
    };
}