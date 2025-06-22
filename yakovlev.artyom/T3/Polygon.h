#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iosfwd>

struct Point
{
    int x{};
    int y{};

    bool operator==(const Point& other) const;
};

std::istream& operator>>(std::istream& in, Point& pt);

struct Polygon
{
    std::vector<Point> points;

    size_t vertexCount() const;
    double area() const;
    bool isRectangle() const;
    bool isEven() const;
    bool hasVertexCount(size_t n) const;
    bool hasRightAngle() const;

    bool operator==(const Polygon& other) const;
};

std::istream& operator>>(std::istream& in, Polygon& p);
std::ostream& operator<<(std::ostream& out, const Polygon& p);

#endif
