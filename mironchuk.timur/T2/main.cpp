#include "DataStruct.h"
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>

const std::string ERROR_OF_EMPTY_VECTOR =
        "Looks like there is no supported record. Cannot determine input. Test skipped";

int main() {
    std::vector<DataStruct> vector;
    bool has_input = false;

    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(vector)
        );
        has_input = has_input || !vector.empty();

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (!has_input) {
        std::cout << ERROR_OF_EMPTY_VECTOR << std::endl;
        return 0;
    }

    if (vector.empty()) {
        std::cout << "Atleast one supported record type" << std::endl;
        return 0;
    }

    if (vector.size() == 1) {
        std::cout << "Atleast one supported record type" << std::endl;
        return 0;
    }

    std::sort(vector.begin(), vector.end(), compareDataStructs);
    std::copy(
        vector.begin(),
        vector.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}

