#include <vector>
#include <algorithm>

#include "Data_struct.h"

int main()
{
    std::vector<artemonts::DataStruct> data;

    std::istream_iterator<artemonts::DataStruct> start(std::cin);
    std::istream_iterator<artemonts::DataStruct> end;

    std::copy(start, end, std::back_inserter(data));

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::sort(data.begin(), data.end());

    std::ostream_iterator<artemonts::DataStruct> out(std::cout, "\n");
    std::copy(data.begin(), data.end(), out);

    return 0;
}
