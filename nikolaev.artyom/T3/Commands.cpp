#include "Commands.h"

#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std::placeholders;

const std::string ERROR = "<INVALID COMMAND>\n";

artttnik::EvenOddAreaAccumulator::EvenOddAreaAccumulator(bool evenFlag) : even(evenFlag)
{}

double artttnik::EvenOddAreaAccumulator::operator()(double acc, const Polygon &p) const
{
  return (p.points_.size() % 2 == (even ? 0 : 1)) ? acc + calculateArea(p) : acc;
}

artttnik::VertexCountAreaAccumulator::VertexCountAreaAccumulator(size_t count) : vertexCount(count)
{}

double artttnik::VertexCountAreaAccumulator::operator()(double acc, const Polygon &p) const
{
  return (p.points_.size() == vertexCount) ? acc + calculateArea(p) : acc;
}

artttnik::EvenOddPredicate::EvenOddPredicate(bool evenFlag) : even(evenFlag)
{}
bool artttnik::EvenOddPredicate::operator()(const Polygon &p) const
{
  return p.points_.size() % 2 == (even ? 0 : 1);
}

artttnik::VertexCountPredicate::VertexCountPredicate(size_t count) : vertexCount(count)
{}

bool artttnik::VertexCountPredicate::operator()(const Polygon &p) const
{
  return p.points_.size() == vertexCount;
}

bool artttnik::AreaComparator::operator()(const Polygon &a, const Polygon &b) const
{
  return calculateArea(a) < calculateArea(b);
}

bool artttnik::VertexCountComparator::operator()(const Polygon &a, const Polygon &b) const
{
  return a.points_.size() < b.points_.size();
}

void artttnik::processArea(const std::vector<Polygon> &polygons, const std::string &arg)
{
  if (arg == "EVEN" || arg == "ODD")
  {
    bool even = (arg == "EVEN");
    double sum =
        std::accumulate(polygons.begin(), polygons.end(), 0.0, EvenOddAreaAccumulator(even));
    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
  }
  else if (arg == "MEAN")
  {
    if (polygons.empty())
    {
      std::cout << ERROR;
      return;
    }
    double sum =
        std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon &p) { return acc + calculateArea(p); });
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
      double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                                   VertexCountAreaAccumulator(vertexCount));
      std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    }
    catch (const std::invalid_argument &)
    {
      std::cout << ERROR;
    }
    catch (const std::out_of_range &)
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
    auto it = std::max_element(polygons.begin(), polygons.end(), AreaComparator());
    std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << "\n";
  }
  else if (arg == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), VertexCountComparator());
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
    auto it = std::min_element(polygons.begin(), polygons.end(), AreaComparator());
    std::cout << std::fixed << std::setprecision(1) << calculateArea(*it) << "\n";
  }
  else if (arg == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), VertexCountComparator());
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
    size_t count = std::count_if(polygons.begin(), polygons.end(), EvenOddPredicate(even));
    std::cout << count << "\n";
  }
  else
  {
    try
    {
      size_t vertexCount = std::stoul(arg);
      if (vertexCount < 3)
      {
        std::cout << ERROR;
        return;
      }
      size_t count =
          std::count_if(polygons.begin(), polygons.end(), VertexCountPredicate(vertexCount));
      std::cout << count << "\n";
    }
    catch (const std::invalid_argument &)
    {
      std::cout << ERROR;
    }
    catch (const std::out_of_range &)
    {
      std::cout << ERROR;
    }
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

  auto predicate = std::bind([](const Polygon &target, const Polygon &a,
                                const Polygon &b) { return a == target && b == target; },
                             std::cref(target), _1, _2);

  auto newEnd = std::unique(polygons.begin(), polygons.end(), predicate);
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
