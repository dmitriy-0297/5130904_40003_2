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
