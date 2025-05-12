#pragma once
#include "Program.h"

#include <vector>
#include <iostream>
#include <algorithm>

struct VertexCountAreaAccumulator 
{
  int vertexCount;
  VertexCountAreaAccumulator(int count) : vertexCount(count) {}
  double operator()(double acc, const Polygon& p) const {
    return acc + ((static_cast<int>(p.points.size()) == vertexCount) ? polygonArea(p) : 0.0);
  }
};

struct VertexCountComparator
{
  bool operator()(const Polygon& a, const Polygon& b) const {
    return a.points.size() < b.points.size();
  }
};

struct VertexCountChecker
{
  int vertexCount;
  VertexCountChecker(int count) : vertexCount(count) {}
  bool operator()(const Polygon& p) const
  {
    return static_cast<int>(p.points.size()) == vertexCount;
  }
};

struct AreaSumAccumulator
{
  double operator()(double acc, const Polygon& p) const {
    return acc + polygonArea(p);
  }
};

struct EvenOddChecker
{
  bool even;
  EvenOddChecker(bool evenFlag) : even(evenFlag) {}
  bool operator()(const Polygon& p) const {
    return (p.points.size() % 2 == (even ? 0 : 1));
  }
};

struct AreaLess
{
  Polygon polygon;
  AreaLess(const Polygon& p) : polygon(p) {}
  bool operator()(const Polygon& p) const {
    return polygonArea(p) < polygonArea(polygon);
  }
};

struct SequenceInfo
{
  int current;
  int best;
};

struct MaxSeqAccumulator
{
  const Polygon& target;
  int current = 0, best = 0;
  MaxSeqAccumulator(const Polygon& t) : target(t) {}


  SequenceInfo operator()(SequenceInfo st, const Polygon& p) const
  {
    if (arePolygonsEqual(p, target))
    {
      ++st.current;
       st.best = std::max(st.best, st.current);
    }
    else
    {
      st.current = 0;
    }
      return st;
  }
};
