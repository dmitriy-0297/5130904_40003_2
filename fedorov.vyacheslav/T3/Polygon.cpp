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
    return 0.0;

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
    return false;

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
    return false;

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
  return points == other.points;
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
    auto point_it = std::istream_iterator<Point>(is);
    std::vector<Point> temp;
    std::copy_n(point_it, n, std::back_inserter(temp));
    if (temp.size() != n)
    {
      p.points.clear();
      is.setstate(std::ios::failbit);
    }
    else
    {
      p.points = std::move(temp);
    }
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
  for (const auto& pt : p.points)
  {
    os << " (" << pt.x << ';' << pt.y << ')';
  }
  return os;
}