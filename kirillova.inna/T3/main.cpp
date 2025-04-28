#include <iostream>

#include "Polygons.h"
#include "Commands.h"
#include "Exceptions.h"

const std::string ERROR_OF_WRONG_COMMAND = "<INVALID COMMAND>";

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      throw WrongNumberOfArguments();
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
      throw CantOpenFile();
    }

    using input_it = std::istream_iterator<kirillova::Polygon>;
    std::vector<kirillova::Polygon> polygons;

    std::copy(input_it{file}, input_it{}, std::back_inserter(polygons));
    if (file.bad())
    {
      throw CantReadFile();
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

      try
      {
        executeCommands(polygons, command, arguments);
      }
      catch (const std::exception& except)
      {
        std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
      }
    }

    return EXIT_SUCCESS;
  }
  catch (const std::invalid_argument& except)
  {
    std::cerr << except.what() << "\n";
    return EXIT_FAILURE;
  }
  catch (const std::runtime_error& except)
  {
    std::cerr << except.what() << "\n";
    return EXIT_FAILURE;
  }
}
