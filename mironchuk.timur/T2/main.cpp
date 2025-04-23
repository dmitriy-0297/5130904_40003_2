#include <iostream>
#include "DataStruct.h"

int main() {
    auto data = parseData(std::cin);

    if (total_lines_read == 0) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped";
        return 0;
    }

    if (data.size() < 2) {
        std::cout << "Atleast one supported record type";
        return 0;
    }

    sortData(data);
    printData(data, std::cout);
    return 0;
}

