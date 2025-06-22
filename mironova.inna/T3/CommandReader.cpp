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
#include <iomanip>
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
    double area = -1;
    if (isNumber(parameter) && std::stoi(parameter) >= 3)
        area = ph_.getAREA(std::stoi(parameter));
    else if (parameter == "EVEN")
        area = ph_.getAREA(true);
    else if (parameter == "ODD")
        area = ph_.getAREA(false);
    else if (parameter == "MEAN" && !ph_.isEmpty())
        area = ph_.getAREA();
    else area = -1;

    if (area == -1) out_ << INVALID_COMMAND_ERROR << endl;
    else
    {
        out_ << std::fixed << std::setprecision(1);
        out_ << area << endl;
    }
}

void CommandReader::MAX(string parameter)
{
    if (ph_.isEmpty()) out_ << INVALID_COMMAND_ERROR << endl;
    else
    {
        out_ << std::fixed << std::setprecision(1);
        if (parameter == "AREA")
            out_ << ph_.getMAX(true) << endl;
        else if (parameter == "VERTEXES")
            out_ << static_cast<int>(ph_.getMAX(false)) << endl;
        else out_ << INVALID_COMMAND_ERROR << endl;
    }
}

void CommandReader::MIN(string parameter)
{
    if (ph_.isEmpty()) out_ << INVALID_COMMAND_ERROR << endl;
    else
    {
        out_ << std::fixed << std::setprecision(1);
        if (parameter == "AREA")
            out_ << ph_.getMIN(true) << endl;
        else if (parameter == "VERTEXES")
            out_ << static_cast<int>(ph_.getMIN(false)) << endl;
        else out_ << INVALID_COMMAND_ERROR << endl;
    }
}

void CommandReader::COUNT(string parameter)
{
    if (isNumber(parameter))
    {
        int p = std::stoi(parameter);
        if (p < 3) out_ << INVALID_COMMAND_ERROR << endl;
        else out_ << ph_.getCOUNT(std::stoi(parameter)) << endl;
    }
    else if (parameter == "EVEN")
        out_ << ph_.getCOUNT(true) << endl;
    else if (parameter == "ODD")
        out_ << ph_.getCOUNT(false) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::PERMS(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getPERMS(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::MAXSEQ(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getMAXSEQ(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::RMECHO(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getRMECHO(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::ECHO(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getECHO(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::LESSAREA(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getLESSAREA(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::INFRAME(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getINFRAME(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::INTERSECTIONS(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getINTERSECTIONS(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
}

void CommandReader::SAME(string datastring)
{
    if (isPolygon(datastring))
        out_ << ph_.getSAME(datastring) << endl;
    else out_ << INVALID_COMMAND_ERROR << endl;
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
    string commandline;
    string command;
    string parameter;
    if (!getline(in_, commandline)) return false;
    std::istringstream iss(commandline);
    iss >> command;
    if ((command == "AREA") && (iss >> parameter))
        AREA(parameter);
    else if ((command == "MAX") && (iss >> parameter))
        MAX(parameter);
    else if ((command == "MIN") && (iss >> parameter))
        MIN(parameter);
    else if ((command == "COUNT") && (iss >> parameter))
        COUNT(parameter);
    else if ((command == "PERMS") && (getline(iss, parameter)))
        PERMS(parameter);
    else if ((command == "MAXSEQ") && (getline(iss, parameter)))
        MAXSEQ(parameter);
    else if ((command == "RMECHO") && (getline(iss, parameter)))
        RMECHO(parameter);
    else if ((command == "ECHO") && (getline(iss, parameter)))
        ECHO(parameter);
    else if ((command == "LESSAREA") && (getline(iss, parameter)))
        LESSAREA(parameter);
    else if ((command == "INFRAME") && (getline(iss, parameter)))
        INFRAME(parameter);
    else if ((command == "INTERSECTIONS") && (getline(iss, parameter)))
        INTERSECTIONS(parameter);
    else if ((command == "SAME") && (getline(iss, parameter)))
        SAME(parameter);
    else if (command == "RECTS")
        RECTS();
    else if (command == "RIGHTSHAPES")
        RIGHTSHAPES();
    else
    {
        out_ << INVALID_COMMAND_ERROR << endl;
    }

    return true;
}

void CommandReader::runEverything()
{
    while(readCommand());
}
