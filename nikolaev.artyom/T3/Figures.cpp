#include "Figures.h"

#include <algorithm>
#include <climits>
#include <limits>
#include <numeric>

namespace artttnik
{

bool Point::operator==(const Point &other) const
{
  return x_ == other.x_ && y_ == other.y_;
}

bool Point::operator<(const Point &other) const
{
  return x_ < other.x_ || (x_ == other.x_ && y_ < other.y_);
}

bool Polygon::operator==(const Polygon &other) const
{
  if (points_.size() != other.points_.size())
    return false;

  std::vector<Point> thisSorted = points_;
  std::vector<Point> otherSorted = other.points_;
  std::sort(thisSorted.begin(), thisSorted.end());
  std::sort(otherSorted.begin(), otherSorted.end());

  return std::equal(thisSorted.begin(), thisSorted.end(), otherSorted.begin());
}

bool Frame::contains(const Point &point) const
{
  return point.x_ >= minL_.x_ && point.x_ <= maxR_.x_ && point.y_ >= minL_.y_ &&
         point.y_ <= maxR_.y_;
}

bool Frame::contains(const Polygon &poly) const
{
  return std::all_of(poly.points_.begin(), poly.points_.end(),
                     [this](const Point &p) { return contains(p); });
}

double calculateArea(const Polygon &poly)
{
  auto area = [](double sum, const std::pair<Point, Point> &edge) {
    const Point &i = edge.first;
    const Point &j = edge.second;
    return sum + (i.x_ * j.y_) - (j.x_ * i.y_);
  };

  std::vector<std::pair<Point, Point>> edges;
  if (!poly.points_.empty())
  {
    std::transform(poly.points_.begin(), std::prev(poly.points_.end()),
                   std::next(poly.points_.begin()), std::back_inserter(edges),
                   [](const Point &a, const Point &b) { return std::make_pair(a, b); });

    edges.push_back(std::make_pair(poly.points_.back(), poly.points_.front()));
  }

  return std::abs(std::accumulate(edges.begin(), edges.end(), 0.0, area)) / 2.0;
}

bool checkPointsInFrame(const std::vector<Point> &points, const Frame &frame, size_t index)
{
  if (index >= points.size())
    return true;
  if (!frame.contains(points[index]))
    return false;
  return checkPointsInFrame(points, frame, index + 1);
}

Frame updateFrameWithPolygon(const Polygon &poly, const Frame &current, size_t point_index)
{
  if (point_index >= poly.points_.size())
  {
    return current;
  }

  const Point &point = poly.points_[point_index];
  Frame updated = {
    {std::min(current.minL_.x_, point.x_), std::min(current.minL_.y_, point.y_)},
    {std::max(current.maxR_.x_, point.x_), std::max(current.maxR_.y_, point.y_)}};

  return updateFrameWithPolygon(poly, updated, point_index + 1);
}

Frame getBoundingFrameRecursive(const std::vector<Polygon>& polygons, size_t poly_index)
{
  if (poly_index == polygons.size())
  {
    const int max = std::numeric_limits<int>::max();
    const int min = std::numeric_limits<int>::min();
    return Frame{{max, max}, {min, min}};
  }

  Frame next_frame = getBoundingFrameRecursive(polygons, poly_index + 1);
  return updateFrameWithPolygon(polygons[poly_index], next_frame, 0);
}

}
