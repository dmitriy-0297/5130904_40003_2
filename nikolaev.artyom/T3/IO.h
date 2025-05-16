#ifndef IO_H
#define IO_H

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "Figures.h"

namespace artttnik
{

std::istream &operator>>(std::istream &in, Point &point);
std::istream &operator>>(std::istream &in, Polygon &poly);
std::vector<Polygon> readPolygonsFromFile(const std::string &filename);
bool readPointsRecursively(std::istream& iss, std::vector<Point>& points, std::size_t remaining);
bool readPolygonFromStream(std::istream &input, Polygon &target);

}

#endif
