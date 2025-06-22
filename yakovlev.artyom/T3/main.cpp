#include "Commands.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: filename not provided\n";
        return EXIT_FAILURE;
    }
    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: cannot open file\n";
        return EXIT_FAILURE;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Polygon p;
        if (iss >> p)
        {
            polygons.push_back(p);
        }
    }
    if (file.bad())
    {
        std::cerr << "Error: reading file\n";
        return EXIT_FAILURE;
    }

    executeCommands(polygons);
    return EXIT_SUCCESS;
}
