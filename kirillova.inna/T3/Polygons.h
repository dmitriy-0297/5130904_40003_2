#ifndef POLYGONS_H
#define POLYGONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <regex>
#include <functional>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <iomanip>

namespace kirillova
{
  struct Point
  {
    int x, y;
    bool operator==(const Point& other) const;
  };

  struct Polygon
  {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
  };

  double getPolygonsArea(const Polygon& polygon);
  size_t getPolygonSize(const Polygon& polygon);

  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Point& p);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
}

#endif
