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


std::istream& operator>>(std::istream& in, Point& point)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  std::string token;
  in >> token;

  static const std::regex pattern(R"(\((-?\d+);(-?\d+)\))");
  std::smatch match;

  if (!std::regex_match(token, match, pattern))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  point.x = std::stoi(match[1]);
  point.y = std::stoi(match[2]);

  return in;
}

std::istream& operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  poly.points.clear();
  std::size_t n = 0;
  in >> n;

  if (in.fail() || n < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  poly.points.reserve(n);

  for (std::size_t i = 0; i < n; ++i)
  {
    Point p;
    if (!(in >> p))
    {
      in.setstate(std::ios::failbit);
      return in;
    }
      poly.points.push_back(p);
  }

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
  std::string extra;
  if (input >> extra) {
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
  std::string extra;
  if (input >> extra) {
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
    std::cerr << "Error: cannot open file" << '\n';
    std::exit(1);
  }

  std::vector<Polygon> result;
  std::string line;

  while (std::getline(fin, line))
  {
    if (line.empty()) continue;

    std::istringstream iss(line);
    Polygon p;

    if (!(iss >> p)) continue;


    std::string extra;
    if (iss >> extra) continue;

    result.push_back(std::move(p));
  }

  return result;
}

void processCommands(const std::vector<Polygon>& all)
{
  std::string line;
  if (!std::getline(std::cin, line))
     return;

  if (line.empty())
    return processCommands(all);

  std::istringstream iss(line);
  std::string cmd, param;
  iss >> cmd >> param;

  if (cmd == "MIN")
  {
    if (param == "AREA")
    {
      if (all.empty())
        std::cout << "<INVALID COMMAND>\n";
      else
        std::cout << std::fixed << std::setprecision(1) << getMinArea(all) << "\n";
    }
    else if (param == "VERTEXES")
    {
      if (all.empty())
        std::cout << "<INVALID COMMAND>\n";
      else
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
    else if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit))
    {
      int v = std::stoi(param);
      if (v < 3)
        std::cout << "<INVALID COMMAND>\n";
      else
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
    else if (!param.empty() && std::all_of(param.begin(), param.end(), ::isdigit))
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
    if (param.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    else
    {
      std::string rest;
      std::getline(iss, rest);
      std::istringstream sub(param + " " + rest);
      getPolygonsLessArea(all, sub);
    }
  }
  else if (cmd == "MAXSEQ")
  {
    if (param.empty())
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    else
    {
      std::string rest;
      std::getline(iss, rest);
      std::istringstream sub(param + " " + rest);
      getMaxSequence(all, sub);
    }
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
    processCommands(all);
}
