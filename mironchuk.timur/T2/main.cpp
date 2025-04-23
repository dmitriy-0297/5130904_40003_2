#include <iostream>
#include "DataStruct.h"

int main() {
    try {
        auto data = parseData(std::cin);
        sortData(data);
        printData(data, std::cout);
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 2;
    }
    return 0;
}
