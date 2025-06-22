#ifndef COMMAND_READER_H
#define COMMAND_READER_H
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

using namespace std;

struct CommandReader
{
    PolygonHolder ph_;
    std::ifstream& file_;
    std::istream& in_;
    std::ostream& out_;

    //конструктор
    CommandReader(std::ifstream& file, std::istream& in, std::ostream& out);

    //вспомогательные
    bool isNumber(const string& s);
    bool isPolygon(const string& datastring);
    void checkAndAdd(const string& datastring);

    //команды
    void AREA(string parameter);
    void MAX(string parameter);
    void MIN(string parameter);
    void COUNT(string parameter);
    void PERMS(string datastring);
    void MAXSEQ(string datastring);
    void RMECHO(string datastring);
    void ECHO(string datastring);
    void LESSAREA(string datastring);
    void INFRAME(string datastring);
    void INTERSECTIONS(string datastring);
    void SAME(string datastring);
    void RECTS();
    void RIGHTSHAPES();


    //чтение файла
    void readFile();
    bool readCommand();
    void runEverything();
};

#endif
