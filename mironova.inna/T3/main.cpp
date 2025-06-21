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
const char* INVALID_FILE = "<INVALID FILE>";
const char* INVALID_POLYGON_ERROR = "<INVALID POLYGON>";


using namespace std;

int main(int, char* argv[])
{
    try
    {
        if (argv[1] == nullptr) throw std::runtime_error(NO_FILE_ERROR);
        std::ifstream inputFile(argv[1]);
        if (!inputFile) throw std::runtime_error(NO_FILE_ERROR);
        CommandReader cr(inputFile, cin, cout);
        cr.readFile();
        cr.runEverything();
        return 0;
    }
    catch (const InvalidPolygon&)
    {
        throw std::runtime_error(INVALID_POLYGON_ERROR);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << INVALID_FILE << std::endl;
        return 1;
    }
}
