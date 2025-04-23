#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "DataStruct.hpp"
#include "record_sort.hpp"

static const char* MSG_EMPTY =
    "Looks like there is no supported record. Cannot determine input. Test skipped";
static const char* MSG_ONE = "Atleast one supported record type";

int main()
{
    std::vector<DataStruct> records;
    while(!std::cin.eof()){
        std::copy(std::istream_iterator<DataStruct>(std::cin),
                  std::istream_iterator<DataStruct>(),
                  std::back_inserter(records));
        if(std::cin.fail() && !std::cin.eof()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }

    if(records.empty()){
        std::cout << MSG_EMPTY << '\n';
        return 0;
    }
    if(records.size()==1){
        std::cout << MSG_ONE << '\n';
        return 0;
    }

    std::sort(records.begin(),records.end(),compareRecords);
    std::copy(records.begin(),records.end(),
              std::ostream_iterator<DataStruct>(std::cout,"\n"));
    return 0;
}

