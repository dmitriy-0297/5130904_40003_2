#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>
#include <cctype>

#include "Data_struct.h"

int main()
{
    using yakovlevart::DataStruct;
    std::vector<DataStruct> data;

    while (true) {
        char c;
        while (std::cin.get(c)) {
            if (c == '(') {
                std::cin.putback(c);
                break;
            }
        }

        if (std::cin.eof()) break;

        DataStruct tmp;
        if (std::cin >> tmp) {
            data.push_back(tmp);
        }
        else if (!std::cin.eof()) {
            std::cin.clear();
            while (std::cin.get(c) && c != ')') {}
        }
    }

    std::sort(data.begin(), data.end());
    std::copy(data.begin(), data.end(),
        std::ostream_iterator<DataStruct>{std::cout, "\n"});

    return 0;
}
