#include <iostream>

#include "Polygons.h"
#include "Commands.h"

const std::string ERROR_OF_WRONG_COMMAND = "<INVALID COMMAND>";
const std::string ERROR_WRONG_NUMBER_OF_ARGUMENTS = "ERROR: should be 2 arguments!";
const std::string ERROR_OF_OPENING_FILE = "ERROR: can't open the file!";
const std::string ERROR_OF_READING_FILE = "ERROR: can't read the file!";

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << ERROR_WRONG_NUMBER_OF_ARGUMENTS << "\n";
    return EXIT_FAILURE;
  }

  std::ifstream file(argv[1]);

  if (!file)
  {
    std::cerr << ERROR_OF_OPENING_FILE << "\n";
    return EXIT_FAILURE;
  }

  using input_it = std::istream_iterator<kirillova::Polygon>;
  std::vector<kirillova::Polygon> polygons;

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    kirillova::Polygon poly;
    if (iss >> poly)
    {
      polygons.push_back(poly);
    }
  }

  if (file.bad())
  {
    std::cerr << ERROR_OF_READING_FILE << "\n";
    return EXIT_FAILURE;
  }

  if (file.fail() && !file.eof())
  {
    std::cerr << ERROR_OF_READING_FILE << "\n";
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

    try
    {
      executeCommands(polygons, command, arguments);
    }
    catch (const std::invalid_argument& except)
    {
      std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
    }
  }

  return EXIT_SUCCESS;
}
