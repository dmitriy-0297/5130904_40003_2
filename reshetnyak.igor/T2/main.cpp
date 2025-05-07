#include "DataStruct.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <cstdlib>

int main()
{
    std::vector<DataStruct> dataVector;

    try {
        std::ios_base::iostate original_state = std::cin.rdstate(); // Сохраняем текущее состояние потока

        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(dataVector)
        );

        if (std::cin.fail() && !std::cin.eof()) { // Проверка на ошибки cin.fail, кроме eof
            std::cin.clear(original_state | std::ios_base::eofbit);
            throw std::runtime_error("Error reading from input");
        }

        if (dataVector.empty()) {
            std::cout << "Empty dataVector\n";
        }
        else {
            std::sort(dataVector.begin(), dataVector.end(), compare);

            std::copy(
                dataVector.begin(),
                dataVector.end(),
                std::ostream_iterator<DataStruct>(std::cout, "\n")
            );
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
