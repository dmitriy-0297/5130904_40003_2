#include "DataStruct.h"

int main()
{
    std::vector<DataStruct> dataVector;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(dataVector)
    );

    if (dataVector.empty())
    {
        std::cout << "Empty dataVector";
    }

    std::sort(dataVector.begin(), dataVector.end(), compare);

    std::copy(dataVector.begin(), dataVector.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return EXIT_SUCCESS;
}