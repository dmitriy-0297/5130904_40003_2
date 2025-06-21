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

    char c;
    while (std::cin.get(c)) {
        if (c == '(') {
            std::cin.putback(c);

            DataStruct tmp;
            if (std::cin >> tmp) {
                data.push_back(tmp);
            }
            else {
                std::cin.clear();
                while (std::cin.get(c) && c != ')') {}
            }
        }
        else if (c != ' ' && c != '\n' && c != '\t') {
            while (std::cin.get(c) && c != '(') {}
            if (c == '(') {
                std::cin.putback(c);
            }
        }
    }

    std::sort(data.begin(), data.end());
    std::copy(data.begin(), data.end(),
        std::ostream_iterator<DataStruct>{std::cout, "\n"});

    return 0;
}
