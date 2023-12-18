#include <ctime>
#include <string>
#include <iostream>
//#include "assets/options.h"
#include "pricers/BSModel.h"


namespace QLabs {
    int main(int argc, char** argv) {
        std::string who = "world";
        Option option;
        std::cout << option.getPremium() << std::endl;

        return EXIT_SUCCESS;
    }
}