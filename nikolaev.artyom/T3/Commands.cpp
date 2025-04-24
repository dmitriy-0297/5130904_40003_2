#include "Commands.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>

const std::string ERROR_INVALID_COMMAND = "<INVALID COMMAND>";

namespace artttnik
{

VertexParity::VertexParity(bool even) : even_(even)
{}

bool VertexParity::operator()(const Polygon &poly) const
{
  return (poly.points_.size() % 2 == 0) == even_;
}

VertexCount::VertexCount(std::size_t count) : count_(count)
{}

bool VertexCount::operator()(const Polygon &poly) const
{
  return poly.points_.size() == count_;
}

void handleAreaCommand(const std::vector<Polygon> &polygons, const std::string &arg)
{
  std::function<bool(const Polygon &)> predicate;

  if (arg == "EVEN")
  {
    predicate = VertexParity(true);
  }
  else if (arg == "ODD")
  {
    predicate = VertexParity(false);
  }
  else if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      std::cout << "0.0\n";
      return;
    }
    double totalArea =
        std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon &poly) { return acc + calculateArea(poly); });
    double mean = totalArea / static_cast<double>(polygons.size());
    std::cout << std::fixed << std::setprecision(1) << mean << "\n";
    return;
  }
  else if (arg == "VERTEXES")
  {
    std::istringstream iss(arg);
    std::size_t target = 0;
    iss >> target;
    predicate = VertexCount(target);
  }
  else
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return;
  }

  auto accumulateFunc = std::bind(
      [](double acc, const Polygon &poly, const std::function<bool(const Polygon &)> &pred) {
        return pred(poly) ? acc + calculateArea(poly) : acc;
      },
      std::placeholders::_1, std::placeholders::_2, predicate);

  double result = std::accumulate(polygons.begin(), polygons.end(), 0.0, accumulateFunc);

  std::cout << std::fixed << std::setprecision(1) << result << "\n";
}

void handleMaxCommand(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (polygons.empty())
  {
    std::cout << 0 << std::endl;
    return;
  }

  if (arg == "AREA")
  {
    auto it =
        std::max_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
          return calculateArea(a) < calculateArea(b);
        });
    std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << "\n";
  }
  else if (arg == "VERTEXES")
  {
    auto compareAreas = std::bind(
        [](const Polygon &a, const Polygon &b) { return calculateArea(a) < calculateArea(b); },
        std::placeholders::_1, std::placeholders::_2);

    auto it = std::max_element(polygons.begin(), polygons.end(), compareAreas);
    std::cout << it->points_.size() << "\n";
  }
  else
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
  }
}

void handleMinCommand(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (polygons.empty())
  {
    std::cout << "0\n";
    return;
  }

  if (arg == "AREA")
  {
    auto it =
        std::min_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
          return calculateArea(a) < calculateArea(b);
        });
    std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << "\n";
  }
  else if (arg == "VERTEXES")
  {
    auto it =
        std::min_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
          return a.points_.size() < b.points_.size();
        });
    std::cout << it->points_.size() << "\n";
  }
  else
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
  }
}

void handleCountCommand(const std::vector<Polygon> &polygons, const std::string &arg)
{
  std::function<bool(const Polygon &)> predicate;

  if (arg == "EVEN")
  {
    predicate = VertexParity(true);
  }
  else if (arg == "ODD")
  {
    predicate = VertexParity(false);
  }
  else if (arg == "VERTEXES")
  {
    std::istringstream iss(arg);
    std::size_t target = 0;
    iss >> target;
    predicate = VertexCount(target);
  }
  else
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return;
  }

  std::size_t count = std::count_if(polygons.begin(), polygons.end(), predicate);
  std::cout << count << "\n";
}

bool readPoints(size_t remaining, std::vector<Point> &points)
{
  if (remaining == 0)
  {
    return true;
  }

  char ch;
  Point point;

  std::cin >> std::ws >> ch;
  if (ch != '(')
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return false;
  }

  if (!(std::cin >> point.x_))
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return false;
  }

  std::cin >> std::ws >> ch;
  if (ch != ';')
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return false;
  }

  if (!(std::cin >> point.y_))
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return false;
  }

  std::cin >> std::ws >> ch;
  if (ch != ')')
  {
    std::cerr << ERROR_INVALID_COMMAND << std::endl;
    return false;
  }

  points.push_back(point);
  return readPoints(remaining - 1, points);
}

void handleInframeCommand(const std::vector<Polygon>& polygons)
{
  if (polygons.empty())
  {
    std::cout << "<FALSE>\n";
    return;
  }

  size_t count;
  if (!(std::cin >> count))
  {
    std::cout << ERROR_INVALID_COMMAND << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
  }

  std::vector<Point> points;
  if (!readPoints(count, points))
  {
    std::cout << ERROR_INVALID_COMMAND << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
  }

  Polygon target{points};
  Frame frame = getBoundingFrameRecursive(polygons);

  bool inside = checkPointsInFrame(target.points_, frame);
  std::cout << (inside ? "<TRUE>" : "<FALSE>") << "\n";
}

void removeDuplicates(std::vector<Polygon> &polygons, const Polygon &target, size_t &removed_count)
{
  std::vector<Polygon> result;
  bool previous = false;

  for (size_t i = 0; i < polygons.size(); ++i)
  {
    bool current = (polygons[i] == target);

    if (!(current && previous))
    {
      result.push_back(polygons[i]);
    }
    else
    {
      ++removed_count;
    }

    previous = current;
  }

  polygons = result;
}

void handleRmechoCommand(std::vector<Polygon> &polygons)
{
  if (polygons.empty())
  {
    std::cout << "0\n";
    return;
  }

  size_t count;
  if (!(std::cin >> count))
  {
    std::cout << "ERROR_INVALID_COMMAND" << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
  }

  std::vector<Point> points;
  if (!readPoints(count, points))
  {
    std::cout << ERROR_INVALID_COMMAND << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return;
  }

  Polygon target{points};
  size_t removed = 0;

  removeDuplicates(polygons, target, removed);

  std::cout << removed << "\n";
}

void processCommands(std::vector<Polygon> &polygons)
{
  std::string command;

  if (!(std::cin >> command))
  {
    if (std::cin.eof())
    {
      exit(EXIT_SUCCESS);
    }
    std::cout << ERROR_INVALID_COMMAND << std::endl;
    return;
  }

  try
  {
    if (command == "AREA")
    {
      std::string arg;
      if (std::cin >> arg)
      {
        handleAreaCommand(polygons, arg);
      }
      else
      {
        throw std::invalid_argument(ERROR_INVALID_COMMAND);
      }
    }
    else if (command == "MAX")
    {
      std::string arg;
      if (std::cin >> arg)
      {
        handleMaxCommand(polygons, arg);
      }
      else
      {
        throw std::invalid_argument(ERROR_INVALID_COMMAND);
      }
    }
    else if (command == "MIN")
    {
      std::string arg;
      if (std::cin >> arg)
      {
        handleMinCommand(polygons, arg);
      }
      else
      {
        throw std::invalid_argument(ERROR_INVALID_COMMAND);
      }
    }
    else if (command == "COUNT")
    {
      std::string arg;
      if (std::cin >> arg)
      {
        handleCountCommand(polygons, arg);
      }
      else
      {
        throw std::invalid_argument(ERROR_INVALID_COMMAND);
      }
    }
    else if (command == "RMECHO")
    {
      handleRmechoCommand(polygons);
    }

    else if (command == "INFRAME")
    {
      handleInframeCommand(polygons);
    }
    else
    {
      throw std::invalid_argument(ERROR_INVALID_COMMAND);
    }
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << e.what() << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  processCommands(polygons);
}

}
