#include <iostream>
#include <iterator>
#include <limits>
#include "DataStruct.h"

static const std::string EMPTY_MSG =
    "Looks like there is no supported record. Cannot determine input. Test skipped";
static const std::string ONE_MSG = "Atleast one supported record type";

int main() {
    if (std::cin.peek() == EOF) {
        std::cout << EMPTY_MSG << '\n';
        return 0;
    }

    auto data = parseData(std::cin);

    if (data.empty()) {
        std::cout << EMPTY_MSG << '\n';
        return 0;
    }
    if (data.size() == 1) {
        std::cout << ONE_MSG << '\n';
        return 0;
    }

    sortData(data);
    printData(data, std::cout);
    return 0;
}

