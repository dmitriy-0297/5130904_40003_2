#include "Program.h"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "You should use ./lab <filename>" << "\n";
    return EXIT_FAILURE;
  }

  auto all = readPolygonsFromFile(argv[1]);
  processCommands(all);
  return EXIT_SUCCESS;
}
