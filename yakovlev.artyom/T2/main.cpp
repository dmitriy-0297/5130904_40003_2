#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "Data_struct.h"

int main()
{
    std::vector<yakovlevart::DataStruct> data;
    std::copy(std::istream_iterator<yakovlevart::DataStruct>{std::cin},
        std::istream_iterator<yakovlevart::DataStruct>{},
        std::back_inserter(data));

    std::sort(data.begin(), data.end());

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<yakovlevart::DataStruct>{std::cout, "\n"});
    return 0;
}
