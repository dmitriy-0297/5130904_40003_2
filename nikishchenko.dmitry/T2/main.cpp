#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>
#include "DataStruct.h"

//В7
void cleanVector(std::vector<DataStruct>& vector);

int main()
{
    std::vector<DataStruct> dataVector;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(dataVector)
    );
    cleanVector(dataVector);
    std::sort(
        dataVector.begin(),
        dataVector.end(),
        compareDataStruct
    );
    std::copy(
        dataVector.begin(),
        dataVector.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}

void cleanVector(std::vector<DataStruct>& vector)
{
    bool mask = false;
    std::vector<DataStruct>::iterator iter;
    for (iter = vector.begin(); iter != vector.end();)
    {
        mask = (*iter).key1 == INVALID_DOUBLE_INDICATOR
            && (*iter).key2 == INVALID_LL_INDICATOR
            && (*iter).key3 == INVALID_STR_INDICATOR;
        if (mask)
        {
            iter = vector.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}
