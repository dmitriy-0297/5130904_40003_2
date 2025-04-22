#include "DataStruct.h"

void cleanVector(std::vector<DataStruct>& vector) {
    vector.erase(
        std::remove_if(vector.begin(), vector.end(),
            [](const DataStruct& ds) {
                return ds.key1 == INVALID_LL_INDICATOR &&
                    ds.key2 == INVALID_CMP_INDICATOR &&
                    ds.key3 == INVALID_STR_INDICATOR;
            }),
        vector.end()
    );
}

int main() {
    std::vector<DataStruct> dataVector;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(dataVector)
    );

    cleanVector(dataVector);

    if (dataVector.empty()) {
        std::cerr << "Empty dataVector\n";
        return 0;
    }

    std::sort(dataVector.begin(), dataVector.end(), compareDataStruct);

    std::copy(
        dataVector.begin(),
        dataVector.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}
