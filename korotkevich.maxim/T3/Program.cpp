#include "Program.h"
#include "Functors.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <regex>

double polygonArea(const Polygon& poly)
{
  auto areaAccum = [](double sum, const std::pair<Point, Point>& edge)
  {
    const Point& i = edge.first;
    const Point& j = edge.second;
    return sum + (static_cast<double>(i.x) * j.y) - (static_cast<double>(j.x) * i.y);
  };

  std::vector<std::pair<Point, Point>> edges;
  const auto& pts = poly.points;
  if (!pts.empty())
  {
    std::transform(
      pts.begin(), std::prev(pts.end()),
      std::next(pts.begin()),
      std::back_inserter(edges),
      [](const Point& a, const Point& b)
      {
        return std::make_pair(a, b);
      }
  );
  edges.emplace_back(pts.back(), pts.front());
  }

  double sum = std::accumulate(edges.begin(), edges.end(), 0.0, areaAccum);
  return std::abs(sum) * 0.5;
}


bool arePolygonsEqual(const Polygon& a, const Polygon& b)
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }

  return std::equal(
    a.points.begin(), a.points.end(),
    b.points.begin(),
    [](const Point& p1, const Point& p2)
    {
      return p1.x == p2.x && p1.y == p2.y;
    }
    );
}


std::istream& operator>>(std::istream& in, Polygon& poly)
{
 std::istream::sentry sentry(in);
 if (!sentry)
 {
   return in;
 }

  std::string line;
  std::getline(in, line);
  std::istringstream iss(line);

  int count;
  if (!(iss >> count) || count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::vector<Point> points;
  points.reserve(count);

  std::string rest;
  std::getline(iss, rest);

  static const std::regex point_regex(R"(\((-?\d+);(-?\d+)\))");
  auto begin = std::sregex_iterator(rest.begin(), rest.end(), point_regex);
  auto end = std::sregex_iterator();

  for (auto it = begin; it != end; ++it)
  {
    if (points.size() >= static_cast<size_t>(count))
      break;

    int x = std::stoi((*it)[1]);
    int y = std::stoi((*it)[2]);
    points.push_back({ x, y });
  }

  if (points.size() != static_cast<size_t>(count))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  poly.points = std::move(points);
  return in;
}


void processAreaEvenOdd(const std::vector<Polygon>& figures, bool even)
{
  std::vector<Polygon> filtered;
  std::copy_if(figures.begin(), figures.end(), std::back_inserter(filtered), [even](const Polygon& p)
  {
    return (p.points.size() % 2 == 0) == even;
  });

  std::vector<double> areas;
  areas.reserve(filtered.size());
  std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), polygonArea);

  double total = std::inner_product(areas.begin(), areas.end(), areas.begin(), 0.0,
    std::plus<double>(), [](double a, double) { return a; });

  std::cout << std::fixed << std::setprecision(1) << total << "\n";
}


void processAreaMean(const std::vector<Polygon>& all)
{
  if (all.empty())
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  double total = std::accumulate(
    all.begin(), all.end(), 0.0,
    AreaSumAccumulator()
  );
  double mean = total / all.size();
  std::cout << std::fixed << std::setprecision(1) << mean << "\n";
}


void processAreaVertexCount(const std::vector<Polygon>& figures, int v)
{
  if (v < 3)
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }
  double sum = std::accumulate(
    figures.begin(), figures.end(), 0.0,
    VertexCountAreaAccumulator(v)
  );
  std::cout << std::fixed << std::setprecision(1) << sum << "\n";
}

double getMaxArea(const std::vector<Polygon>& figures)
{
  std::vector<double> areas;
  areas.reserve(figures.size());

  std::transform(
    figures.begin(), figures.end(), std::back_inserter(areas), std::bind(polygonArea, std::placeholders::_1)
  );
  return *std::max_element(areas.begin(), areas.end());
}



double getMinArea(const std::vector<Polygon>& figures)
{
  std::vector<double> areas;
  areas.reserve(figures.size());

  std::transform(
    figures.begin(), figures.end(), std::back_inserter(areas), std::bind(polygonArea, std::placeholders::_1)
  );
  return *std::min_element(areas.begin(), areas.end());
}


int getMaxVertexes(const std::vector<Polygon>& figures)
{
  std::vector<int> counts;
  counts.reserve(figures.size());

  std::transform(figures.begin(), figures.end(), std::back_inserter(counts), [](const Polygon& p) {return p.points.size(); });
  return *std::max_element(counts.begin(), counts.end());
}


int getMinVertexes(const std::vector<Polygon>& figures)
{
  auto it = std::min_element(figures.begin(), figures.end(), VertexCountComparator());
  return static_cast<int>(it->points.size());
}

int countEvenOdd(const std::vector<Polygon>& figures, bool evOd)
{
  return std::count_if(figures.begin(), figures.end(), EvenOddChecker(evOd));
}

int countByVertexCount(const std::vector<Polygon>& figures, int v)
{
  return std::count_if(figures.begin(), figures.end(), VertexCountChecker(v));
}

void getPolygonsLessArea(const std::vector<Polygon>& figures, std::istream& input)
{
  Polygon polygon;
  if (!(input >> polygon))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  std::vector<bool> lessFlags(figures.size());
  std::transform(
    figures.begin(),
    figures.end(),
    lessFlags.begin(),
    AreaLess(polygon)
  );

  int count = std::count(lessFlags.begin(), lessFlags.end(), true);
  std::cout << count << '\n';
}



void getMaxSequence(const std::vector<Polygon>& figures, std::istream& input)
{
  Polygon target;
  if (!(input >> target))
  {
    std::cout << "<INVALID COMMAND>\n";
    return;
  }

  SequenceInfo init{ 0, 0 };
  SequenceInfo result = std::accumulate(
    figures.begin(), figures.end(),
    init,
    MaxSeqAccumulator(target)
  );

  std::cout << result.best << "\n";
}


std::vector<Polygon> readPolygonsFromFile(const char* filename)
{
  std::ifstream fin(filename);
  if (!fin)
  {
    std::cerr << "Error: cannot open file '" << filename << "'\n";
    std::exit(EXIT_FAILURE);
  }
  std::vector<Polygon> all;
  std::copy(
    std::istream_iterator<Polygon>(fin),
    std::istream_iterator<Polygon>(),
    std::back_inserter(all)
  );
  return all;
}

void processCommands(const std::vector<Polygon>& all)
{
  std::string cmd, param;
  if (!(std::cin >> cmd >> param))
  {
    return;
  }

  if (cmd == "MIN")
  {
    if (all.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    else if (param == "AREA")
    {
      std::cout << std::fixed << std::setprecision(1) << getMinArea(all) << "\n";
    }
    else if (param == "VERTEXES")
    {
      std::cout << getMinVertexes(all) << "\n";
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  else if (cmd == "MAX")
  {
    if (param == "AREA")
    {
      if (all.empty())
        std::cout << "<INVALID COMMAND>\n";
      else
        std::cout << std::fixed << std::setprecision(1) << getMaxArea(all) << "\n";
    }
    else if (param == "VERTEXES")
    {
      if (all.empty())
        std::cout << "<INVALID COMMAND>\n";
      else
        std::cout << getMaxVertexes(all) << "\n";
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  else if (cmd == "COUNT")
  {
    if (param == "EVEN")
    {
      std::cout << countEvenOdd(all, true) << "\n";
    }
    else if (param == "ODD")
    {
      std::cout << countEvenOdd(all, false) << "\n";
    }
    else if (std::all_of(param.begin(), param.end(), ::isdigit))
    {
      int v = std::stoi(param);
      std::cout << countByVertexCount(all, v) << "\n";
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  else if (cmd == "AREA")
  {
    if (param == "EVEN")
    {
      processAreaEvenOdd(all, true);
    }
    else if (param == "ODD")
    {
      processAreaEvenOdd(all, false);
    }
    else if (param == "MEAN")
    {
      processAreaMean(all);
    }
    else if (std::all_of(param.begin(), param.end(), ::isdigit))
    {
      processAreaVertexCount(all, std::stoi(param));
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  else if (cmd == "LESSAREA")
  {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(param + " " + line);
    getPolygonsLessArea(all, iss);
  }
  else if (cmd == "MAXSEQ")
  {
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(param + " " + line);
    getMaxSequence(all, iss);
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
    std::string skip;
    std::getline(std::cin, skip);
  }

  processCommands(all);
}
