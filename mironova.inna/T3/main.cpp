#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <fstream>
#include "Point.h"
#include "Polygon.h"
#include "PolygonHolder.h"
#include "CommandReader.h"

const char* NO_FILE_ERROR = "<NO FILE>";
const char* INVALID_POLYGON_ERROR = "<INVALID POLYGON>";


using namespace std;

int main(int, char* argv[])
{
    try
    {
        std::ifstream inputFile(argv[1]);
        if (!inputFile) throw std::runtime_error(NO_FILE_ERROR);
        CommandReader cr(inputFile, cin, cout);
        cr.readFile();
        cr.runEverything();
    }
    catch (const InvalidPolygon&)
    {
        throw std::runtime_error(INVALID_POLYGON_ERROR);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
