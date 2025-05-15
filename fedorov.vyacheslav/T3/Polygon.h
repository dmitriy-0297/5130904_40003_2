#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <iosfwd>

struct Point 
{
  int x, y;
  bool operator==(const Point& other) const;
};

struct Polygon 
{
  std::vector<Point> points;
  size_t vertexCount() const;
  double area() const;
  bool isRectangle() const;
  bool isEven() const;
  bool hasVertexCount(size_t n) const;
  bool operator==(const Polygon& other) const;
};

std::istream& operator>>(std::istream&, Polygon&);
std::ostream& operator<<(std::ostream&, const Polygon&);

#endif