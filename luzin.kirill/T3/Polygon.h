#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <vector>

#define ODD 1
#define EVEN -1
#define ALL 0
#define NUM 2
#define LESS_AREA 3

struct Polygon
{
  struct Point
  {
	  int x, y;
  };

  std::vector< Point > points;

  Polygon() {};

  void readPoint(std::istream& stream, int n);
  Point inputPoint(std::istream& stream);
  int getNumberPoints() const;
  float getArea() const;
  bool checkIntersect(Point p1, Point q1, Point p2, Point q2) const;

  bool isOdd() const;
  bool isEven() const;
  bool isEqualNumberOfPoint(const int& num) const;
  bool isIntersected(const Polygon& other) const;

  friend bool inputPolygon(std::istream& stream, Polygon& polygon);
  friend std::istream& operator >> (std::istream& stream, Polygon& polygon);
  friend std::ostream& operator << (std::ostream& stream, const Polygon& polygon);
};

std::vector< float > getAllAreas(const std::vector< Polygon >& polygons, const int& typeSum, const int& n = 1);
std::vector< int > getAllWithPredicator(const std::vector< Polygon >& polygons, const int& typeSum, const float& n = 1);

#endif

