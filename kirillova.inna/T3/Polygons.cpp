#include "Polygons.h"

namespace kirillova
{
  bool Point::operator==(const Point& other) const
  {
    return (x == other.x && y == other.y);
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    return points == other.points;
  }

  double getPolygonsArea(const Polygon& polygon)
  {
    double area = 0.0;
    const auto& points = polygon.points;

    for (size_t i = 0; i < points.size(); ++i)
    {
      size_t j = (i + 1) % points.size();
      area += (points[i].x * points[j].y) - (points[j].x * points[i].y);
    }

    return std::abs(area) / 2.0;
  }

  size_t getPolygonSize(const Polygon& polygon)
  {
    return polygon.points.size();
  }

  /*Polygon normalizePolygonsPoints(const Polygon& polygon)
  {
    if (polygon.points.empty()) return polygon;

    Polygon normalizedPolygon = polygon;
    int dx = polygon.points[0].x;
    int dy = polygon.points[0].y;

    for (auto& p : normalizedPolygon.points)
    {
      p.x -= dx;
      p.y -= dy;
    }

    return normalizedPolygon;
  }*/

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    polygon.points.clear();
    std::string line;
    if (!std::getline(in, line))
    {
      return in;
    }

    std::regex poly_re(R"(^\s*(\d+)(\s*\(\s*(-?\d+)\s*;\s*(-?\d+)\s*\)\s*)+$)");
    if (!std::regex_match(line, poly_re))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::istringstream iss(line);
    size_t n;
    iss >> n;
    for (size_t i = 0; i < n; ++i)
    {
      Point p;
      char c1, c2, c3;
      if (!(iss >> c1 >> p.x >> c2 >> p.y >> c3) || c1 != '(' || c2 != ';' || c3 != ')')
      {
        in.setstate(std::ios::failbit);
        break;
      }
      polygon.points.push_back(p);
    }
    return in;
  }

  std::ostream& operator<<(std::ostream& out, const Point& p)
  {
    return out << "(" << p.x << ";" << p.y << ")";
  }

  std::ostream& operator<<(std::ostream& out, const Polygon& polygon)
  {
    out << polygon.points.size() << " ";
    for (const auto& p : polygon.points)
    {
      out << p << " ";
    }
    return out;
  }
}
