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
#include <map>
#include <functional>
#include "Point.h"
#include "Polygon.h"
#include "PolygonHolder.h"
#include "CommandReader.h"

const int FILE_CHUNK_SIZE = 1000;
const char* INVALID_COMMAND_ERROR = "<INVALID COMMAND>";

using namespace std;

CommandReader::CommandReader(std::ifstream& file, std::istream& in, std::ostream& out) : file_(file), in_(in), out_(out)
{
    PolygonHolder ph;
    ph_ = ph;
}

//вспомогательные
bool CommandReader::isNumber(const string& s)
{
    try
    {
        std::size_t pos;
        std::stoi(s, &pos);
        return pos == s.size();
    }
    catch (...)
    {
        return false;
    }
}

bool CommandReader::isPolygon(const string& datastring)
{
    try
    {
        Polygon p(datastring);
        return true;
    }
    catch (const InvalidPolygon&)
    {
        return false;
    }
}

void CommandReader::checkAndAdd(const string& datastring)
{
    try
    {
        ph_.add(Polygon(datastring));
    }
    catch (const InvalidPolygon&){}
}

//комманды
void CommandReader::AREA(string parameter)
{
    if (isNumber(parameter))
        out_ << ph_.getAREA(std::stoi(parameter)) << endl;
    else if (parameter == "EVEN")
        out_ << ph_.getAREA(true) << endl;
    else if (parameter == "ODD")
        out_ << ph_.getAREA(false) << endl;
    else if (parameter == "MEAN")
        out_ << ph_.getAREA() << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::MAX(string parameter)
{
    if (parameter == "AREA")
        out_ << ph_.getMAX(true) << endl;
    else if (parameter == "VERTEXES")
        out_ << ph_.getMAX(false) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::MIN(string parameter)
{
    if (parameter == "AREA")
        out_ << ph_.getMIN(true) << endl;
    else if (parameter == "VERTEXES")
        out_ << ph_.getMIN(false) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::COUNT(string parameter)
{
    if (isNumber(parameter))
        out_ << ph_.getCOUNT(std::stoi(parameter)) << endl;
    else if (parameter == "EVEN")
        out_ << ph_.getCOUNT(true) << endl;
    else if (parameter == "ODD")
        out_ << ph_.getCOUNT(false) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::PERMS(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getPERMS(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::MAXSEQ(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getMAXSEQ(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::RMECHO(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getRMECHO(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::ECHO(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getECHO(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::LESSAREA(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getLESSAREA(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::INFRAME(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getINFRAME(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::INTERSECTIONS(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getINTERSECTIONS(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::SAME(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getSAME(datastring) << endl;
    else throw std::runtime_error(INVALID_COMMAND_ERROR);
}

void CommandReader::RECTS()
{
    out_ << ph_.getRECTS() << endl;
}

void CommandReader::RIGHTSHAPES()
{
    out_ << ph_.getRIGHTSHAPES() << endl;
}

//основное
void CommandReader::readFile()
{
    std::vector<int> counter(FILE_CHUNK_SIZE);
    std::iota(counter.begin(), counter.end(), 0);

    bool endReached = std::accumulate(
        counter.begin(), counter.end(),
        false,
        [this](bool, int)
        {
            string line;
            if (getline(file_, line))
            {
                checkAndAdd(line);
                return false;
            }
            return true;
        }
    );
    if (!endReached) readFile();
}

bool CommandReader::readCommand()
{
    string command;
    string parameter;
    if (!(in_ >> command)) return false;
    if ((command == "AREA") && (in_ >> parameter))
        AREA(parameter);
    else if ((command == "MAX") && (in_ >> parameter))
        MAX(parameter);
    else if ((command == "MIN") && (in_ >> parameter))
        MIN(parameter);
    else if ((command == "COUNT") && (in_ >> parameter))
        COUNT(parameter);
    else if ((command == "PERMS") && (getline(in_, parameter)))
        PERMS(parameter);
    else if ((command == "MAXSEQ") && (getline(in_, parameter)))
        MAXSEQ(parameter);
    else if ((command == "RMECHO") && (getline(in_, parameter)))
        RMECHO(parameter);
    else if ((command == "ECHO") && (getline(in_, parameter)))
        ECHO(parameter);
    else if ((command == "LESSAREA") && (getline(in_, parameter)))
        LESSAREA(parameter);
    else if ((command == "INFRAME") && (getline(in_, parameter)))
        INFRAME(parameter);
    else if ((command == "INTERSECTIONS") && (getline(in_, parameter)))
        INTERSECTIONS(parameter);
    else if ((command == "SAME") && (getline(in_, parameter)))
        SAME(parameter);
    else if (command == "RECTS")
        RECTS();
    else if (command == "RIGHTSHAPES")
        RIGHTSHAPES();
    else throw std::runtime_error(INVALID_COMMAND_ERROR);

    return true;
}

void CommandReader::runEverything()
{
    while(readCommand());
}
