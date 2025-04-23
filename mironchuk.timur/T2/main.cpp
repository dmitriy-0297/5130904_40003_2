#include <iostream>
#include "DataStruct.h"

static constexpr const char *MSG_EMPTY =
        "Looks like there is no supported record. Cannot determine input. Test skipped";
static constexpr const char *MSG_ONE = "Atleast one supported record type";

int main() {
    if (std::cin.peek() == EOF) {
        std::cout << MSG_EMPTY << '\n';
        return 0;
    }

    auto records = parseData(std::cin);

    if (records.empty()) {
        std::cout << MSG_EMPTY << '\n';
        return 0;
    }
    if (records.size() == 1) {
        std::cout << MSG_ONE << '\n';
        return 0;
    }

    sortData(records);
    printData(records, std::cout);
    return 0;
}

