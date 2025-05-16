#ifndef COMMANDS_H
#define COMMANDS_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

#include "Figures.h"
#include "IO.h"

namespace artttnik
{

struct EvenOddAreaAccumulator {
  bool even;
  explicit EvenOddAreaAccumulator(bool evenFlag);
  double operator()(double acc, const Polygon &p) const;
};

struct VertexCountAreaAccumulator
{
  size_t vertexCount;
  explicit VertexCountAreaAccumulator(size_t count);
  double operator()(double acc, const Polygon &p) const;
};

struct EvenOddPredicate {
  bool even;
  explicit EvenOddPredicate(bool evenFlag);
  bool operator()(const Polygon &p) const;
};

struct VertexCountPredicate {
  size_t vertexCount;
  explicit VertexCountPredicate(size_t count);
  bool operator()(const Polygon &p) const;
};

struct AreaComparator {
  bool operator()(const Polygon &a, const Polygon &b) const;
};

struct VertexCountComparator {
  bool operator()(const Polygon &a, const Polygon &b) const;
};

void processArea(const std::vector<Polygon> &polygons, const std::string &arg);
void processMax(const std::vector<Polygon> &polygons, const std::string &arg);
void processMin(const std::vector<Polygon> &polygons, const std::string &arg);
void processCount(const std::vector<Polygon> &polygons, const std::string &arg);
void processInframe(std::istream &input, const std::vector<Polygon> &polygons);
void processRmecho(std::istream &input, std::vector<Polygon> &polygons);
void chooseCommand(const std::string &command, std::vector<Polygon> &polygons,
  std::istream &input, const std::string &arg = "");
void processCommands(std::vector<Polygon>& polygons);

}

#endif
