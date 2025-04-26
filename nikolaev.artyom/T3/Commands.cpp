#include "Commands.h"

#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std::placeholders;

const std::string ERROR = "<INVALID COMMAND>\n";

void artttnik::processArea(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (arg == "EVEN" || arg == "ODD")
  {
    bool even = (arg == "EVEN");
    auto areaCalculator = std::bind(
        [](bool even, double acc, const Polygon &p) {
          return (p.points_.size() % 2 == (even ? 0 : 1)) ? acc + calculateArea(p) : acc;
        },
        even, _1, _2);
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaCalculator);
    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
  }
  else if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      std::cout << ERROR;
      return;
    }
    auto areaCalculator =
        std::bind([](double acc, const Polygon &p) { return acc + calculateArea(p); }, _1, _2);
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaCalculator);
    std::cout << std::fixed << std::setprecision(1) << sum / polygons.size() << "\n";
  }
  else
  {
    size_t vertexCount;
    try
    {
      vertexCount = std::stoul(arg);
      if (vertexCount < 3)
      {
        std::cout << ERROR;
        return;
      }
      auto areaCalculator = std::bind(
          [](size_t vertexCount, double acc, const Polygon &p) {
            return (p.points_.size() == vertexCount) ? acc + calculateArea(p) : acc;
          },
          vertexCount, _1, _2);
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0, areaCalculator);
      std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    }
    catch (const std::invalid_argument&)
    {
      std::cout << ERROR;
    }
    catch (const std::out_of_range&)
    {
      std::cout << ERROR;
    }
  }
}

void artttnik::processMax(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (polygons.empty())
  {
    std::cout << ERROR;
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
    auto it =
        std::max_element(polygons.begin(), polygons.end(), [](const Polygon &a, const Polygon &b) {
          return a.points_.size() < b.points_.size();
        });
    std::cout << it->points_.size() << "\n";
  }
  else
  {
    std::cout << ERROR;
  }
}

void artttnik::processMin(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (polygons.empty())
  {
    std::cout << ERROR;
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
    std::cout << ERROR;
  }
}

void artttnik::processCount(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (arg == "EVEN" || arg == "ODD")
  {
    bool even = (arg == "EVEN");
    auto predicate = std::bind(
        [](bool evenFlag, const Polygon &p) { return p.points_.size() % 2 == (evenFlag ? 0 : 1); },
        even, _1);

    size_t count = std::count_if(polygons.begin(), polygons.end(), predicate);
    std::cout << count << "\n";
  }
  else
  {
    if (arg.empty() || !std::all_of(arg.begin(), arg.end(), ::isdigit) ||
        (arg.size() > 1 && arg[0] == '0'))
    {
      std::cout << ERROR;
      return;
    }

    char *endPtr = nullptr;
    unsigned long vertexCount = std::strtoul(arg.c_str(), &endPtr, 10);

    if (*endPtr != '\0' || vertexCount < 3)
    {
      std::cout << ERROR;
      return;
    }

    auto predicate = std::bind(
        [](size_t count, const Polygon &p) { return p.points_.size() == count; }, vertexCount, _1);

    size_t count = std::count_if(polygons.begin(), polygons.end(), predicate);
    std::cout << count << "\n";
  }
}

void artttnik::processInframe(std::istream &input, const std::vector<Polygon> &polygons)
{
  Polygon poly;
  if (!readPolygonFromStream(input, poly))
  {
    std::cout << ERROR;
    return;
  }

  Frame boundingFrame = getBoundingFrameRecursive(polygons, 0);
  if (boundingFrame.contains(poly))
    std::cout << "<TRUE>\n";
  else
    std::cout << "<FALSE>\n";
}

void artttnik::processRmecho(std::istream &input, std::vector<Polygon> &polygons)
{
  Polygon target;
  if (!readPolygonFromStream(input, target))
  {
    std::cout << ERROR;
    return;
  }

  auto newEnd =
      std::unique(polygons.begin(), polygons.end(), [&target](const Polygon &a, const Polygon &b) {
        return a == target && b == target;
      });

  size_t removed = std::distance(newEnd, polygons.end());
  polygons.erase(newEnd, polygons.end());

  std::cout << removed << "\n";
}

void artttnik::chooseCommand(const std::string &command, std::vector<Polygon> &polygons,
                             std::istream &input, const std::string &arg)
{
  if (command == "AREA")
    processArea(polygons, arg);
  else if (command == "MAX")
    processMax(polygons, arg);
  else if (command == "MIN")
    processMin(polygons, arg);
  else if (command == "COUNT")
    processCount(polygons, arg);
  else if (command == "INFRAME" && arg.empty())
    processInframe(input, polygons);
  else if (command == "RMECHO" && arg.empty())
    processRmecho(input, polygons);
  else
    std::cout << ERROR;
}

void artttnik::processCommands(std::vector<Polygon> &polygons)
{
  std::string line;
  if (!std::getline(std::cin, line))
  {
    return;
  }

  if (line.empty())
  {
    processCommands(polygons);
    return;
  }

  std::istringstream iss(line);
  std::string command;
  iss >> command;

  if (command == "INFRAME" || command == "RMECHO")
  {
    chooseCommand(command, polygons, iss);
  }
  else
  {
    std::string arg;
    iss >> arg;
    if (iss.eof())
    {
      chooseCommand(command, polygons, iss, arg);
    }
    else
    {
      std::cout << ERROR;
    }
  }

  processCommands(polygons);
}
