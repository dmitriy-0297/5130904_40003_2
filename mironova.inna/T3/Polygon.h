#ifndef POLYGON_H
#define POLYGON_H
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

using namespace std;

struct InvalidPolygon {};

struct Polygon
{
    vector<Point> points_;

    //конструкторы
    Polygon();
    Polygon(const vector<Point>& pts);
    Polygon (string data);

    //операции
    Polygon operator+(const Polygon& other);
    bool operator==(const Polygon& other) const;
    int getVertexes() const;
    bool isEven() const;
    double getArea() const;
    vector<Point> sortPoints() const;
    bool isAlmostEqual(const Polygon& other) const;

    //вспомогательные
    Polygon getSquare();
    bool fitsInFrame(const Point& p) const;
    bool isVertex(const Point& p);
    bool isInside(const Point& p);
    bool isIntersecting(const Polygon& other);
    bool isSame(const Polygon& other);
    bool isRectangle();
    bool hasRightAngles();

};

#endif
