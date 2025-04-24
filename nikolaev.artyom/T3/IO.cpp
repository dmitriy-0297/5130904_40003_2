#include "IO.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

namespace artttnik
{
std::istream &operator>>(std::istream &input, Point &point)
{
  char ch;
  return input >> ch >> point.x_ >> ch >> point.y_ >> ch;
}

std::istream &operator>>(std::istream &input, Polygon &poly)
{
  size_t count;
  if (!(input >> count))
  {
    poly.points_.clear();
    return input;
  }

  std::vector<Point> temp;
  temp.reserve(count);

  std::copy_n(std::istream_iterator<Point>(input), count, std::back_inserter(temp));

  if (temp.size() == count)
  {
    poly.points_ = std::move(temp);
  }
  else
  {
    poly.points_.clear();
  }

  return input;
}

std::vector<Polygon> readPolygonsFromFile(const std::string &filename)
{
  std::ifstream file(filename);

  std::vector<Polygon> polygons;
  std::transform(std::istream_iterator<Polygon>(file), std::istream_iterator<Polygon>(),
                 std::back_inserter(polygons), [](Polygon p) { return p; });

  polygons.erase(std::remove_if(polygons.begin(), polygons.end(),
                                [](const Polygon &p) { return p.points_.empty(); }),
                 polygons.end());

  return polygons;
}

}
