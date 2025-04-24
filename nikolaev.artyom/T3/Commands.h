#ifndef COMMANDS_AREA_H
#define COMMANDS_AREA_H

#include "Figures.h"

#include <string>

namespace artttnik
{
struct VertexParity
{
  bool even_;
  explicit VertexParity(bool even);

  bool operator()(const Polygon &poly) const;
};

struct VertexCount
{
  std::size_t count_;
  explicit VertexCount(std::size_t count);

  bool operator()(const Polygon &poly) const;
};

void handleAreaCommand(const std::vector<Polygon> &polygons, const std::string &arg);
void handleMaxCommand(const std::vector<Polygon> &polygons, const std::string &arg);
void handleMinCommand(const std::vector<Polygon> &polygons, const std::string &arg);
void handleCountCommand(const std::vector<Polygon> &polygons, const std::string &arg);
bool readPoints(size_t remaining, std::vector<Point> &points);
void removeDuplicates(std::vector<Polygon> &polygons, const Polygon &target, size_t &removed_count);

void handleInframeCommand(const std::vector<Polygon> &polygons);
void handleRmechoCommand(std::vector<Polygon> &polygons);

void processCommands(std::vector<Polygon> &polygons);

}

#endif // COMMANDS_AREA_H
