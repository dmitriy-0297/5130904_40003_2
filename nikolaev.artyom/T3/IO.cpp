#include "IO.h"

std::istream &artttnik::operator>>(std::istream &in, Point &point)
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

  point.x_ = std::stoi(match[1].str());
  point.y_ = std::stoi(match[2].str());

  return in;
}

bool artttnik::readPointsRecursively(std::istream& iss, std::vector<Point>& points, std::size_t remaining)
{
  if (remaining == 0)
    return true;

  Point p;
  if (!(iss >> p))
    return false;

  points.push_back(p);
  return readPointsRecursively(iss, points, remaining - 1);
}

std::istream& artttnik::operator>>(std::istream& in, Polygon& poly)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  poly.points_.clear();

  std::size_t n = 0;
  in >> n;

  if (in.fail() || n < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::string line;
  std::getline(in >> std::ws, line);

  std::istringstream iss(line);
  poly.points_.reserve(n);

  if (!readPointsRecursively(iss, poly.points_, n))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  iss >> std::ws;
  if (!iss.eof())
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::vector<artttnik::Polygon> artttnik::readPolygonsFromFile(const std::string &filename)
{
  std::vector<Polygon> polygons;
  std::ifstream file(filename);

  std::function<void()> readNext = [&file, &polygons, &readNext]() {
    std::string line;
    if (!std::getline(file, line)) {
      return;
    }

    std::istringstream iss(line);
    Polygon poly;
    if (iss >> poly) {
      polygons.push_back(poly);
    }

    readNext();
  };

  readNext();
  return polygons;
}

bool artttnik::readPolygonFromStream(std::istream &input, Polygon &target)
{
  std::string line;
  if (!std::getline(input, line))
  {
    return false;
  }

  if (line.empty())
  {
    return false;
  }

  std::istringstream iss(line);
  Polygon poly;
  if (iss >> poly)
  {
    target = std::move(poly);
    return true;
  }

  return false;
}
