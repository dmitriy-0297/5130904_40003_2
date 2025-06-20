#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

#include "Data_struct.h"

int main() {
    std::vector<yakovlevart::DataStruct> data;

    std::istream_iterator<yakovlevart::DataStruct> first(std::cin);
    std::istream_iterator<yakovlevart::DataStruct> last;

    std::copy(first, last, std::back_inserter(data));

    std::sort(data.begin(), data.end());

    std::ostream_iterator<yakovlevart::DataStruct> out(std::cout, "\n");
    std::copy(data.begin(), data.end(), out);

    return 0;
}