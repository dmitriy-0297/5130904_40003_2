#ifndef FIGURES_H
#define FIGURES_H

#include <cmath>
#include <vector>

namespace artttnik
{
struct Point
{
  int x_;
  int y_;

  bool operator==(const Point &other) const;
  bool operator<(const Point &other) const;
  Point() = default;
  Point(int x, int y);
};

struct Polygon
{
  std::vector<Point> points_;

  bool operator==(const Polygon &other) const;
  Polygon() = default;
  Polygon(const std::vector<Point> &points);
};

struct Frame
{
  Point minL_;
  Point maxR_;

  bool contains(const Point &point) const;
  bool contains(const Polygon &poly) const;
};

double calculateArea(const Polygon &poly);
Frame updateFrameWithPolygon(const Polygon &poly, const Frame &current, size_t point_index);
Frame getBoundingFrameRecursive(const std::vector<Polygon> &polygons, size_t poly_index = 0);

}

#endif // FIGURES_H
