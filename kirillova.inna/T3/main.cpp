#include <iostream>
#include "Polygons.h"
#include "Commands.h"
#include "Exceptions.h"

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      throw WrongNumberOfArguments();
    }
  }
  catch (const std::invalid_argument& except)
  {
    std::cerr << except.what() << "\n";
    return EXIT_FAILURE;
  }

  std::ifstream file(argv[1]);

  try
  {
    if (!file)
    {
      throw CantOpenFile();
    }
  }
  catch (const std::runtime_error& except)
  {
    std::cerr << except.what() << "\n";
    return EXIT_FAILURE;
  }

  using input_it = std::istream_iterator<kirillova::Polygon>;
  std::vector<kirillova::Polygon> polygons;

  try
  {
    std::copy(input_it{ file }, input_it{}, std::back_inserter(polygons));
    if (file.bad())
    {
      throw CantReadFile();
    }
  }
  catch (const std::runtime_error& except)
  {
    std::cerr << except.what() << "\n";
    return EXIT_FAILURE;
  }

  std::string commandLine;
  while (std::getline(std::cin, commandLine))
  {
    std::istringstream iss(commandLine);
    std::string command;
    std::string arguments;

    iss >> command;
    std::getline(iss, arguments);
    if (!arguments.empty() && arguments.front() == ' ')
    {
      arguments.erase(0, 1);
    }

    executeCommands(polygons, command, arguments);
  }

  return EXIT_SUCCESS;
}
