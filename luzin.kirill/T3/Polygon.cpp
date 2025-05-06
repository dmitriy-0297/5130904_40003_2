#include "Polygon.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <limits>

using namespace std::placeholders;

#define ERROR_INVALID_INPUT "ERROR: Invalid input"
#define ERROR_INVALID_FINISH_OF_STRING "ERROR: String must be finished with \\n"

Polygon::Point Polygon::inputPoint(std::istream& stream)
{
  Point point;

  if(stream.get() != ' ')
  {
    throw std::invalid_argument(ERROR_INVALID_INPUT);
  }

  if(stream.get() != '(')
  {
    throw std::invalid_argument(ERROR_INVALID_INPUT);
  }

  stream >> point.x;

  if(stream.fail())
  {
    throw std::invalid_argument(ERROR_INVALID_INPUT);
  }

  if(stream.get() != ';')
  {
    throw std::invalid_argument(ERROR_INVALID_INPUT);
  }

  stream >> point.y;

  if(stream.fail())
  {
    throw std::invalid_argument(ERROR_INVALID_INPUT);
  }

  if(stream.get() != ')')
  {
    throw std::invalid_argument(ERROR_INVALID_INPUT);
  }

  return point;
}

void Polygon::readPoint(std::istream& stream, int n)
{
  if(n <= 0)
  {
    if (stream.peek() != '\r' && stream.peek() != '\n' && !stream.eof())
    {
      std::cout << static_cast< int >(stream.peek());
      throw std::invalid_argument(ERROR_INVALID_FINISH_OF_STRING);
    }

    return;
  }

  points.push_back(inputPoint(stream));

  readPoint(stream, n - 1);
}

int Polygon::getNumberPoints() const
{
  return static_cast< int >(points.size());
}

float Polygon::getArea() const
{
  float area = 0;
  std::vector<int> xPoints;
  std::vector<int> yPoints;

  std::transform(points.begin(),
    points.end(),
    std::back_inserter(xPoints),
    [](const Point& point) { return point.x; } );

  std::transform(points.begin(),
    points.end(),
    std::back_inserter(yPoints),
    [](const Point& point) { return point.y; } );

  std::vector< int > positive;
  std::copy(yPoints.begin() + 1, yPoints.end(), std::back_inserter(positive));
  positive.push_back(yPoints[0]);

  std::transform(positive.begin(), positive.end(), xPoints.begin(), positive.begin(), std::multiplies<int>());

  std::vector< int > negative;
  std::copy(xPoints.begin()+1, xPoints.end(), std::back_inserter(negative));
  negative.push_back(xPoints[0]);

  std::transform(negative.begin(), negative.end(), yPoints.begin(), negative.begin(), std::multiplies<int>());

  area = static_cast<float>(std::accumulate(positive.begin(), positive.end(), area, std::plus< int >()));
  area = static_cast<float>(std::accumulate(negative.begin(), negative.end(), area, std::minus< int >()));

  return std::abs(static_cast< float >(area / 2.0));
}

bool Polygon::isIntersected(const Polygon& other) const
{
  int i = 0;
  int j = 0;

  return std::any_of(points.begin(), points.end(), [&](Point p) {
    j = 0;
    Point p1 = points[i++];
    Point q1 = points[(i) % points.size()];
    return std::any_of(other.points.begin(), other.points.end(), [&](Point q) {
      Point p2 = other.points[j++];
      Point q2 = other.points[(j) % other.points.size()];
      return checkIntersect(p1, q1, p2, q2);
      });
    });
}

bool Polygon::checkIntersect(Point p1, Point q1, Point p2, Point q2) const
{
  auto getOrientation = [](Point p, Point q, Point r)
    {
      int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
      if (val == 0)
      {
        return 0;
      }
      else if (val > 0)
      {
        return 1;
      }
      else
      {
        return -1;
      }
    };

  int orientation1 = getOrientation(p1, q1, p2);
  int orientation2 = getOrientation(p1, q1, q2);
  int orientation3 = getOrientation(p2, q2, p1);
  int orientation4 = getOrientation(p2, q2, q1);

  if (orientation1 != orientation2 && orientation3 != orientation4) 
  {
    return true;
  }

  if (orientation1 == 0 || orientation2 == 0 || orientation3 == 0 || orientation4 == 0) 
  {
    return true;
  }

  return false;
}


bool inputPolygon(std::istream& stream, Polygon& polygon)
{
  int count = 0;
  stream >> count;

  try
  {
    polygon.readPoint(stream, count);
  }
  catch(const std::invalid_argument& error)
  {
    std::cerr << error.what() << '\n';
    polygon.points.clear();
    stream.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

    inputPolygon(stream, polygon);
  }

  return true;
}

std::istream& operator >> (std::istream& stream, Polygon& polygon)
{
  polygon.points.clear();

  inputPolygon(stream, polygon);

  return stream;
}

std::ostream& operator << (std::ostream& stream, const Polygon& polygon)
{
  stream << "->";

  std::for_each(polygon.points.begin(),
    polygon.points.end(),
    [&stream](const Polygon::Point& point) { stream << '(' << point.x << ';' << point.y << ") "; } );

    stream << '\n';

    return stream;
}


bool Polygon::isOdd() const
{
  return (getNumberPoints() % 2 == 1);
}

bool Polygon::isEven() const
{
  return (getNumberPoints() % 2 == 0);
}

bool Polygon::isEqualNumberOfPoint(const int& num) const
{
  return (getNumberPoints() == num);
}

std::vector< float > getAllAreas(const std::vector< Polygon >& polygons, const int& typeSum, const int& n)
{
  std::vector< float > areas;

  switch (typeSum)
  {
  case EVEN:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(areas),
      [](const Polygon& polygon) { return (polygon.isEven() ? polygon.getArea() : 0); });
    break;

  case ODD:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(areas),
      [](const Polygon& polygon) { return (polygon.isOdd() ? polygon.getArea() : 0); });
    break;

  case ALL:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(areas),
      [](const Polygon& polygon) { return polygon.getArea(); });
    break;

  case NUM:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(areas),
      [&n](const Polygon& polygon) { return (polygon.isEqualNumberOfPoint(n) ? polygon.getArea() : 0); });
    break;

  default:
    break;
  }

  return areas;
}

std::vector< int > getAllWithPredicator(const std::vector< Polygon >& polygons, const int& typeSum, const float& n)
{
  std::vector< int > numbers;

  switch (typeSum)
  {
  case EVEN:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(numbers),
      [](const Polygon& polygon) { return (polygon.isEven() ? 1 : 0); });
    break;

  case ODD:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(numbers),
      [](const Polygon& polygon) { return (polygon.isOdd() ? 1 : 0); });
    break;

  case ALL:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(numbers),
      [](const Polygon& polygon) { return 1; });
    break;

  case LESS_AREA:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(numbers),
      [&n](const Polygon& polygon) { return (polygon.getArea() < n ? 1 : 0); });
    break;

  case NUM:
    std::transform(polygons.begin(),
      polygons.end(),
      std::back_inserter(numbers),
      [&n](const Polygon& polygon) { return (polygon.isEqualNumberOfPoint(n) ? 1 : 0); });
    break;

  default:
    break;
  }

  return numbers;
}