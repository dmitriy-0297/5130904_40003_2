#include <iostream>
#include <vector>
#include <iterator>
#include "DataStruct.h"

int main() {
  std::vector<krrmaxim::DataStruct> dataVector;

  std::copy(
    std::istream_iterator<krrmaxim::DataStruct>(std::cin),
    std::istream_iterator<krrmaxim::DataStruct>(),
    std::back_inserter(dataVector)
  );
  std::sort(dataVector.begin(), dataVector.end(), krrmaxim::dataStructComparator);

  std::copy(
    dataVector.begin(),
    dataVector.end(),
    std::ostream_iterator<krrmaxim::DataStruct>(std::cout, "\n")
  );
  return EXIT_SUCCESS;
}
