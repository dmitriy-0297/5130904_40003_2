#include <iostream>
#include "DataStruct.h"

int main() {
    auto data = parseData(std::cin);
    
    std::istreambuf_iterator<char> it(std::cin), end;
    bool no_input = (it == end);
    if (no_input) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    }

    if (data.size() == 1) {
        std::cout << "Atleast one supported record type\n";
        return 0;
    }

    sortData(data);
    printData(data, std::cout);
    return 0;
}


