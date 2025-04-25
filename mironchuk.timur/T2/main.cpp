#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "DataStruct.hpp"
#include "record_sort.hpp"


int main() {
    std::vector<DataStruct> DataStructV;
    while (!std::cin.eof()) {
        std::copy(std::istream_iterator<DataStruct>(std::cin),
                  std::istream_iterator<DataStruct>(), std::back_inserter(DataStructV));
        if (std::cin.fail() && !std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(DataStructV.begin(), DataStructV.end(), compare);

    std::copy(std::begin(DataStructV), std::end(DataStructV), std::ostream_iterator<DataStruct>(std::cout, "\n"));
}

