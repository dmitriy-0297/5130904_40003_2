#ifndef POLYGON_HOLDER_H
#define POLYGON_HOLDER_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include "Point.h"
#include "Polygon.h"


using namespace std;

struct PolygonHolder
{
    vector<Polygon> polygons_;

    //конструкторы
    PolygonHolder(const vector<Polygon>& ps);
    PolygonHolder();

    //вспомогательные
    Polygon getFrame();
    void add(const Polygon& p);
    bool isEmpty();

    //основные
    double getAREA(bool even);
    double getAREA();
    double getAREA(int vertexes);
    double getMAX(bool area);
    double getMIN(bool area);
    int getCOUNT(bool even);
    int getCOUNT(int vertexes);
    int getPERMS(string data);
    int getMAXSEQ(string data);
    int getRMECHO(string data);
    int getECHO(string data);
    int getLESSAREA(string data);
    string getINFRAME(string data);
    int getINTERSECTIONS(string data);
    int getSAME(string data);
    int getRECTS();
    int getRIGHTSHAPES();

};

#endif

