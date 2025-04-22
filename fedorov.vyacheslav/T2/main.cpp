#include "DataStruct.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>

int main() {
  std::vector<DataStruct> vector;
  std::string line;

  while (std::getline(std::cin, line)) {
    if (line.empty()) continue;

    std::istringstream iss(line);
    DataStruct data;
    if (iss >> data) {
      vector.push_back(data);
    }
  }

  std::sort(vector.begin(), vector.end(), cusComparator);

  std::copy(vector.begin(),
    vector.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n"));

  return EXIT_SUCCESS;
}
