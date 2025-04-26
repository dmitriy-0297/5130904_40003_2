#include "IO.h"
#include "Commands.h"

#include <iostream>
#include <vector>
#include <string>

using namespace artttnik;


int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "<INVALID FILE>" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Polygon> polygons = readPolygonsFromFile(argv[1]);

  processCommands(polygons);

  return EXIT_SUCCESS;
}
