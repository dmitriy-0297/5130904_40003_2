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

//конструкторы
Point::Point(int x, int y)
{
     x_ = x;
     y_ = y;
}

Point::Point(string data)
{
    try
    {
        if (data.front() == '(' && data.back() == ')') {
            data = data.substr(1, data.size() - 2);
        }

        std::istringstream iss(data);
        string token;
        std::getline(iss, token, ';');
        int x = std::stoi(token);
        std::getline(iss, token, ';');
        int y = std::stoi(token);
        x_ = x;
        y_ = y;
    }
    catch (...)
    {
        throw InvalidPoint();
    }
}



//сравнение
bool Point::compare(const Point& other) const
{
    if ((x_ == other.x_) && (y_ == other.y_)) return false;

    if (x_ > other.x_) return true;
    if (x_ < other.x_) return false;

    if (y_ > other.y_) return true;
    if (y_ < other.y_) return false;

    return false;
}

bool Point::operator==(const Point& other) const
{
    return ((x_ == other.x_) && (y_ == other.y_));
}

bool Point::operator>(const Point& other) const
{
    return compare(other);
}

bool Point::operator<(const Point& other) const
{
    return other.compare(*this);
}

bool Point::moreX(const Point& other) const
{
    return (x_ > other.x_);
}

bool Point::moreY(const Point& other) const
{
    return (y_ > other.y_);
}

bool Point::lessX(const Point& other) const
{
    return (x_ < other.x_);
}

bool Point::lessY(const Point& other) const
{
    return (y_ < other.y_);
}


//вспомогательные
double Point::getAngle(const Point& a, const Point& b) const
{
    double ax = a.x_ - x_;
    double ay = a.y_ - y_;
    double bx = b.x_ - x_;
    double by = b.y_ - y_;

    double dot = ax * bx + ay * by;
    double cross = ax * by - ay * bx;
    double lenA = std::sqrt(ax * ax + ay * ay);
    double lenB = std::sqrt(bx * bx + by * by);

    if (lenA == 0 || lenB == 0) return 0.0;

    double cosTheta = dot / (lenA * lenB);
    cosTheta = std::max(-1.0, std::min(1.0, cosTheta));

    double angle = std::acos(cosTheta);

    if(cross >= 0) angle = -angle;
    return std::abs((angle*180)/M_PI);
}

Point Point::getDelta(const Point& other) const
{
    return Point(other.x_ - x_, other.y_ - y_);
}

bool Point::isRight(const Point& a, const Point& b) const
{
    double angle = getAngle(a, b);
    return std::abs(angle - 90.0) < 1e-2;
}
