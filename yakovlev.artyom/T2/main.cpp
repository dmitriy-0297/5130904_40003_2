#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

#include "Data_struct.h"

int main()
{
    using yakovlevart::DataStruct;
    std::vector<DataStruct> data;

    while (std::cin.good()) {
        DataStruct tmp;
        if (std::cin >> tmp) {
            data.push_back(tmp);
        }
        else if (!std::cin.eof()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end());
    std::copy(data.begin(), data.end(),
        std::ostream_iterator<DataStruct>{std::cout, "\n"});

    return 0;
}
