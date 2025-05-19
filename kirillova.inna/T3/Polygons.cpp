#include "Polygons.h"

namespace kirillova
{
  bool Point::operator==(const Point& other) const
  {
    return (x == other.x && y == other.y);
  }

  bool Point::operator<(const Point& other) const
  {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }

  bool Polygon::operator==(const Polygon& other) const
  {
    if (points.size() != other.points.size())
    {
      return false;
    }

    std::multiset<Point> thisPoints(points.begin(), points.end());
    std::multiset<Point> otherPoints(other.points.begin(), other.points.end());

    return thisPoints == otherPoints;
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

    return (std::abs(area) / 2.0);
  }

  size_t getPolygonSize(const Polygon& polygon)
  {
    return polygon.points.size();
  }

  bool same_comparator(const Polygon& polygon, const Polygon& other)
  {
    if (polygon.points.size() != other.points.size())
    {
      return false;
    }

    int dx = other.points[0].x - polygon.points[0].x;
    int dy = other.points[0].y - polygon.points[0].y;

    for (size_t i = 1; i < polygon.points.size(); ++i)
    {
      if (other.points[i].x != polygon.points[i].x + dx ||
          other.points[i].y != polygon.points[i].y + dy)
      {
        return false;
      }
    }

    return true;
  }

  std::istream& operator>>(std::istream& in, Polygon& polygon)
  {
    polygon.points.clear();
    std::string line;
    if (!std::getline(in, line))
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    std::istringstream iss(line);
    size_t n;
    if (!(iss >> n) || n < 3)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    for (size_t i = 0; i < n; ++i)
    {
      char c1, c2, c3;
      Point p;
      if (!(iss >> c1 >> p.x >> c2 >> p.y >> c3) || c1 != '(' || c2 != ';' || c3 != ')')
      {
        in.setstate(std::ios::failbit);
        return in;
      }
      polygon.points.push_back(p);
    }

    std::string remaining;
    if (iss >> remaining)
    {
      in.setstate(std::ios::failbit);
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
