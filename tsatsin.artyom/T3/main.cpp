#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>
#include "geometry.h"
#include "commands.h"

int main(int argc, char* argv[])
{
    using artemonts::Polygon;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(1);

    if (argc != 2)
    {
        std::cerr << "<INVALID_ARGUMENT>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "File couldn't be opened " << argv[1] << '\n';
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        Polygon poly;
        if (iss >> poly && iss >> std::ws && iss.eof() && poly.points.size() >= 3)
            polygons.emplace_back(std::move(poly));
    }
    file.close();


    auto cmds = artemonts::makeCommands(polygons);

    std::string cmd;
    while (std::cin >> cmd)
    {
        auto it = cmds.find(cmd);
        if (it == cmds.end())
        {
            std::cout << "<INVALID COMMAND>\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        it->second(std::cin, std::cout);
    }
    return 0;
}
