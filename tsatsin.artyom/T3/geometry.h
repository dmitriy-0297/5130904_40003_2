#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <istream>
#include <ostream>

namespace artemonts
{
    struct Point
    {
        int x;
        int y;
    };

    struct Polygon
    {
        std::vector<Point> points;
    };

    std::istream& operator>>(std::istream& in, Point& p);
    std::ostream& operator<<(std::ostream& out, const Point& p);
    bool operator==(const Point& a, const Point& b);

    std::istream& operator>>(std::istream& in, Polygon& poly);
    bool operator==(const Polygon& a, const Polygon& b);

    double area(const Polygon& poly);
    bool isRectangle(const Polygon& poly);
}

#endif
