#include "IO.h"
#include "Commands.h"
#include <iostream>

using namespace artttnik;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "You should use ./lab <filename>" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Polygon> polygons = readPolygonsFromFile(argv[1]);
  if (polygons.empty())
  {
    std::cerr << "Invalid input" << std::endl;
    return EXIT_FAILURE;
  }

  processCommands(polygons);

  return EXIT_SUCCESS;
}
