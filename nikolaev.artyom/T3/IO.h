#ifndef IO_H
#define IO_H

#include "Figures.h"

#include <istream>

namespace artttnik
{
std::istream &operator>>(std::istream &input, Point &point);

std::istream &operator>>(std::istream &input, Polygon &poly);

std::vector<Polygon> readPolygonsFromFile(const std::string &filename);

}

#endif // IO_H
