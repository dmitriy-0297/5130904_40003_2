#include "Polygon.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <vector>
#include <iterator>
#include <array>

bool Point::operator==(const Point& other) const
{
  return x == other.x && y == other.y;
}

size_t Polygon::vertexCount() const
{
  return points.size();
}

double Polygon::area() const
{
  if (points.empty())
  {
    return 0.0;
  }
  struct Acc
  {
    double sum;
    Point prev;
  };
  Acc initial{ 0.0, points.front() };
  auto result = std::accumulate(
    points.begin() + 1, points.end(), initial,
    [this](Acc acc, const Point& current)
    {
      Acc newAcc;
      newAcc.sum = acc.sum + (acc.prev.x * current.y - current.x * acc.prev.y);
      newAcc.prev = current;
      return newAcc;
    }
  );
  result.sum += (result.prev.x * points.front().y - points.front().x * result.prev.y);
  return std::abs(result.sum) / 2.0;
}

bool Polygon::isRectangle() const
{
  if (vertexCount() != 4)
  {
    return false;
  }
  auto getVector = [](const Point& a, const Point& b)
    {
      return Point{ b.x - a.x, b.y - a.y };
    };
  auto dot = [](const Point& a, const Point& b)
    {
      return a.x * b.x + a.y * b.y;
    };
  std::array<int, 4> indices = { 0, 1, 2, 3 };
  auto check_angle = [&](int i)
    {
      Point a = points[i];
      Point b = points[(i + 1) % 4];
      Point c = points[(i + 2) % 4];
      Point ab = getVector(a, b);
      Point bc = getVector(b, c);
      return dot(ab, bc) == 0;
    };
  if (!std::all_of(indices.begin(), indices.end(), check_angle))
  {
    return false;
  }
  Point a = points[0], b = points[1], c = points[2], d = points[3];
  return (a.x - b.x) == (d.x - c.x) && (a.y - b.y) == (d.y - c.y) &&
    (b.x - c.x) == (a.x - d.x) && (b.y - c.y) == (a.y - d.y);
}

bool Polygon::isEven() const
{
  return vertexCount() % 2 == 0;
}

bool Polygon::hasVertexCount(size_t n) const
{
  return vertexCount() == n;
}

bool Polygon::operator==(const Polygon& other) const
{
  if (points.size() != other.points.size())
  {
    return false;
  }
  auto normalize = [](const std::vector<Point>& pts)
    {
      if (pts.empty())
      {
        return pts;
      }
      auto min_x = std::min_element(pts.begin(), pts.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; })->x;
      auto min_y = std::min_element(pts.begin(), pts.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; })->y;
      std::vector<Point> norm_pts;
      std::transform(pts.begin(), pts.end(), std::back_inserter(norm_pts),
        [min_x, min_y](const Point& pt)
        {
          return Point{ pt.x - min_x, pt.y - min_y };
        });
      return norm_pts;
    };
  std::vector<Point> norm_self = normalize(points);
  std::vector<Point> norm_other = normalize(other.points);
  return norm_self == norm_other;
}

std::istream& operator>>(std::istream& is, Point& pt)
{
  char open, close;
  pt.x = pt.y = 0;
  if (is >> open && open != '(')
  {
    is.unget();
    throw std::invalid_argument("Invalid point format");
  }
  if (is >> pt.x)
  {
    if (is.get() != ';')
    {
      throw std::invalid_argument("Invalid point format");
    }
    if (is >> pt.y)
    {
      if (is >> close && close != ')')
      {
        throw std::invalid_argument("Invalid point format");
      }
    }
    else
    {
      throw std::invalid_argument("Invalid point format");
    }
  }
  else
  {
    throw std::invalid_argument("Invalid point format");
  }
  return is;
}

std::istream& operator>>(std::istream& is, Polygon& p)
{
  size_t n;
  p.points.clear();
  if (is >> n)
  {
    if (n < 3)
    {
      is.setstate(std::ios::failbit);
      throw std::invalid_argument("Invalid polygon format");
    }
    auto point_it = std::istream_iterator<Point>(is);
    std::vector<Point> temp;
    std::copy_n(point_it, n, std::back_inserter(temp));
    if (temp.size() != n)
    {
      p.points.clear();
      is.setstate(std::ios::failbit);
      throw std::invalid_argument("Invalid polygon format");
    }
    is >> std::ws;
    if (!is.eof())
    {
      throw std::invalid_argument("Invalid polygon format");
    }
    p.points = std::move(temp);
  }
  else
  {
    is.setstate(std::ios::failbit);
  }
  return is;
}

std::ostream& operator<<(std::ostream& os, const Polygon& p)
{
  os << p.points.size();
  std::for_each(
    p.points.begin(),
    p.points.end(),
    [&](const Point& pt)
    {
      os << " (" << pt.x << ';' << pt.y << ')';
    }
  );
  return os;
}
