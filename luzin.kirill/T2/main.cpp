#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "DataStruct.h"

bool isLess(const DataStruct& left, const DataStruct& right);

int main()
{
  std::vector< DataStruct > vector;

  std::copy(std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(vector));

  std::sort(vector.begin(), vector.end(), isLess);

  std::copy(vector.begin(),
    vector.end(),
    std::ostream_iterator< DataStruct >(std::cout, "\n"));

  return EXIT_SUCCESS;
}

bool isLess(const DataStruct& left, const DataStruct& right)
{
  if (left.key1 == right.key1)
  {
    float leftKey2 = static_cast< float >(left.key2.first) / left.key2.second;
    float rightKey2 = static_cast< float >(right.key2.first) / right.key2.second;

    if (leftKey2 == rightKey2)
    {
      return left.key3.length() < right.key3.length();
    }
    else
    {
      return leftKey2 < rightKey2;
    }
  }
  else
  {
    return left.key1 < right.key1;
  }
}

