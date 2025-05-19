#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <iostream>

struct Point
{
  int x, y;
};

struct Polygon
{
  std::vector<Point> points;
};

double polygonArea(const Polygon& poly);
bool arePolygonsEqual(const Polygon& a, const Polygon& b);

std::istream& operator>>(std::istream& in, Polygon& poly);
std::istream& operator>>(std::istream& in, Point& point);
void processAreaEvenOdd(const std::vector<Polygon>& figures, bool even);
void processAreaMean(const std::vector<Polygon>& all);
void processAreaVertexCount(const std::vector<Polygon>& figures, int v);
double getMaxArea(const std::vector<Polygon>& figures);
double getMinArea(const std::vector<Polygon>& figures);
int getMaxVertexes(const std::vector<Polygon>& figures);
int getMinVertexes(const std::vector<Polygon>& figures);
int countEvenOdd(const std::vector<Polygon>& figures, bool evOd);
int countByVertexCount(const std::vector<Polygon>& figures, int v);
void getPolygonsLessArea(const std::vector<Polygon>& figures, std::istream& input);
void getMaxSequence(const std::vector<Polygon>& figures, std::istream& input);

std::vector<Polygon> readPolygonsFromFile(const char* filename);
void processCommands(const std::vector<Polygon>& all);

#endif
