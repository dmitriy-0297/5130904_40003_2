#include "DataStruct.h"
#include <vector>
#include <iterator>
#include <algorithm>
#include <stdexcept>

const std::string NO_INPUT_MSG = "Looks like there is no supported record. Cannot determine input. Test skipped";
const std::string ATLEAST_ONE_MSG = "Atleast one supported record type";

int main() {
    std::vector<DataStruct> vector;
    bool has_any_input = false;
    bool has_valid_records = false;

    while (!std::cin.eof()) {
        DataStruct temp;
        std::cin >> temp;

        if (std::cin.fail()) {
            has_any_input = true;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            has_any_input = true;
            has_valid_records = true;
            vector.push_back(temp);
        }
    }

    if (!has_any_input) {
        std::cout << NO_INPUT_MSG << "\n";
        return 0;
    }

    if (!has_valid_records) {
        std::cout << ATLEAST_ONE_MSG << "\n";
        return 0;
    }

    if (vector.size() == 1) {
        std::cout << ATLEAST_ONE_MSG << "\n";
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

