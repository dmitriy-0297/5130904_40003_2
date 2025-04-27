#include "DataStruct.h"

const std::string ERROR_OF_EMPTY_VECTOR = "ERROR: vector is empty, because you wrote incorrect keys!";

int main()
{
  try
  {
    std::vector<kirillova::DataStruct> vector;

    while (!std::cin.eof())
    {
      std::copy(
        std::istream_iterator<kirillova::DataStruct>(std::cin),
        std::istream_iterator<kirillova::DataStruct>(),
        std::back_inserter(vector)
      );
      if (std::cin.fail())
      {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }

    if (vector.empty())
    {
      throw std::logic_error(ERROR_OF_EMPTY_VECTOR);
    }

    std::sort(vector.begin(), vector.end(), kirillova::compareDataStructs);

    std::copy(
      vector.begin(),
      vector.end(),
      std::ostream_iterator<kirillova::DataStruct>(std::cout, "\n")
    );
  }
  catch (const std::logic_error&)
  {
    std::cerr << ERROR_OF_EMPTY_VECTOR << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
