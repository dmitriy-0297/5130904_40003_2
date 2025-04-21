#include <iostream>
#include <iterator>
#include <vector>
#include "DataStruct.h"

int main() {
  std::vector<artttnik::DataStruct> dataVector;

  std::copy(
    std::istream_iterator<artttnik::DataStruct>(std::cin),
    std::istream_iterator<artttnik::DataStruct>(),
    std::back_inserter(dataVector)
  );

  std::sort(dataVector.begin(), dataVector.end(), artttnik::compareDataStruct);

  std::copy(
    dataVector.begin(),
    dataVector.end(),
    std::ostream_iterator<artttnik::DataStruct>(std::cout, "")
  );

  return EXIT_SUCCESS;
}
