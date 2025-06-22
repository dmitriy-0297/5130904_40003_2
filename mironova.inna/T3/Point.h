#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>

using namespace std;

struct InvalidPoint {};

struct Point
{
    int x_;
    int y_;

    //конструкторы
    Point(int x, int y);
    Point(string data);

    //сравнение
    bool compare(const Point& other) const;
    bool operator==(const Point& other) const;
    bool operator>(const Point& other) const;
    bool operator<(const Point& other) const;
    bool moreX(const Point& other) const;
    bool moreY(const Point& other) const;
    bool lessX(const Point& other) const;
    bool lessY(const Point& other) const;

    //вспомогательные
    double getAngle(const Point& a, const Point& b) const;
    Point getDelta(const Point& other) const;
    bool isRight(const Point& a, const Point& b) const;

};

#endif
