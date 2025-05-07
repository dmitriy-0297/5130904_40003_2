#include <iostream>
#include <algorithm>
#include <functional>
#include <iterator>
#include <fstream>
#include <string>
#include <numeric>
#include <limits>
#include <iomanip>

#include "Polygon.h"

#define ERROR_INVALID_COMMAND "<INVALID COMMAND>\n"
#define ERROR_MISSING_FILE "ERROR: Missing file"

#define PRECISION 1

void readFile(const char* fileName, std::vector< Polygon >& polygons);
void readCommand(const std::vector< Polygon >& polygons);
bool isNumber(const std::string& s);

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    std::cerr << "Missing argument filename: " << argv[0] << " 'filename'\n";
    return EXIT_FAILURE;
  }

  std::vector< Polygon > polygons;

  try
  {
    readFile(argv[1], polygons);
  }
  catch (const std::logic_error& error)
  {
    std::cerr << error.what();
    return EXIT_FAILURE;
  }

  readCommand(polygons);

  return EXIT_SUCCESS;
}

void readFile(const char* fileName, std::vector< Polygon >& polygons)
{
  std::ifstream inputFile;

  if (!inputFile.is_open())
  {
    inputFile.open(fileName);
  }

  if (!inputFile.is_open())
  {
    throw std::logic_error(ERROR_MISSING_FILE);
  }

  while (!inputFile.eof() && inputFile.peek() != EOF)
  {
    try
    {
      std::copy(std::istream_iterator< Polygon >(inputFile),
        std::istream_iterator< Polygon >(),
        std::back_inserter(polygons));
    }
    catch (const std::invalid_argument& error)
    {
      inputFile.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

      continue;
    }
  }
}

void readCommand(const std::vector< Polygon >& polygons)
{
  while (!std::cin.eof())
  {
    std::string command;
    std::cin >> command;

    try
    {
      if (command == "AREA")
      {
        std::vector< float > areas;
        std::cin >> command;

        if (command == "EVEN")
        {
          areas = getAllAreas(polygons, EVEN);
        }
        else if (command == "ODD")
        {
          areas = getAllAreas(polygons, ODD);
        }
        else if (isNumber(command))
        {
          int num = std::stoi(command);
          if (num < 3)
          {
            throw std::invalid_argument(ERROR_INVALID_COMMAND);
          }

          areas = getAllAreas(polygons, NUM, num);
        }
        else if (command == "MEAN")
        {
          if (polygons.size() < 1)
          {
            throw std::invalid_argument(ERROR_INVALID_COMMAND);
          }

          areas = getAllAreas(polygons, ALL);
        }
        else
        {
          throw std::invalid_argument(ERROR_INVALID_COMMAND);
        }

        float sum = 0;
        sum = static_cast<float>(std::accumulate(areas.begin(), areas.end(), 0.0));

        if (command == "MEAN")
        {
          sum = sum / polygons.size();
        }

        std::cout << std::fixed << std::setprecision(PRECISION) << sum;
      }
      else if (command == "MAX")
      {
        if (polygons.size() < 1)
        {
          throw std::invalid_argument(ERROR_INVALID_COMMAND);
        }

        std::cin >> command;

        if (command == "AREA")
        {
          std::vector< float > areas;

          areas = getAllAreas(polygons, ALL);
          std::cout << std::fixed << std::setprecision(PRECISION) << *std::max_element(areas.begin(), areas.end());
        }
        else if (command == "VERTEXES")
        {
          std::vector< int > numbersOfPoints;

          std::transform(polygons.begin(),
            polygons.end(),
            std::back_inserter(numbersOfPoints),
            [](const Polygon& polygon) { return polygon.getNumberPoints(); });

          std::cout << *std::max_element(numbersOfPoints.begin(), numbersOfPoints.end());
        }
        else
        {
          throw std::invalid_argument(ERROR_INVALID_COMMAND);
        }
      }
      else if (command == "MIN")
      {
        if (polygons.size() < 1)
        {
          throw std::invalid_argument(ERROR_INVALID_COMMAND);
        }

        std::cin >> command;

        if (command == "AREA")
        {
          std::vector< float > areas;

          areas = getAllAreas(polygons, ALL);
          std::cout << std::fixed << std::setprecision(PRECISION) << *std::min_element(areas.begin(), areas.end());
        }
        else if (command == "VERTEXES")
        {
          std::vector< int > numbersOfPoints;

          std::transform(polygons.begin(),
            polygons.end(),
            std::back_inserter(numbersOfPoints),
            [](const Polygon& polygon) { return polygon.getNumberPoints(); });

          std::cout << *std::min_element(numbersOfPoints.begin(), numbersOfPoints.end());
        }
        else
        {
          throw std::invalid_argument(ERROR_INVALID_COMMAND);
        }
      }
      else if (command == "COUNT")
      {
        std::vector< int > numbersOfPolygons;

        std::cin >> command;

        if (command == "EVEN")
        {
          numbersOfPolygons = getAllWithPredicator(polygons, EVEN);
        }
        else if (command == "ODD")
        {
          numbersOfPolygons = getAllWithPredicator(polygons, ODD);
        }
        else if (isNumber(command))
        {
          int num = std::stoi(command);
          if(num < 3)
          {
            throw std::invalid_argument(ERROR_INVALID_COMMAND);
          }

          numbersOfPolygons = getAllWithPredicator(polygons, NUM, static_cast<float>(num));
        }
        else
        {
          throw std::invalid_argument(ERROR_INVALID_COMMAND);
        }

        std::cout << std::accumulate(numbersOfPolygons.begin(), numbersOfPolygons.end(), 0);
      }
      else if (command == "LESSAREA")
      {
        Polygon poly;
        std::cin >> poly;

        std::vector< int > polygonsWIthLessArea;

        polygonsWIthLessArea = getAllWithPredicator(polygons, LESS_AREA, poly.getArea());

        std::cout << std::accumulate(polygonsWIthLessArea.begin(), polygonsWIthLessArea.end(), 0);
      }
      else if (command == "INTERSECTIONS")
      {
        Polygon poly;
        std::cin >> poly;

        std::vector< int > intersectedPolygons;

        std::transform(polygons.begin(),
          polygons.end(),
          std::back_inserter(intersectedPolygons),
          [&poly](const Polygon& polygon) { return (polygon.isIntersected(poly) ? 1 : 0); });

        std::cout << std::accumulate(intersectedPolygons.begin(), intersectedPolygons.end(), 0);
      }
      else if (std::cin.eof())
      {
        continue;
      }
      else
      {
        throw std::invalid_argument(ERROR_INVALID_COMMAND);
      }

      std::cout << '\n';
    }
    catch (const std::invalid_argument& error)
    {
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      std::cout << error.what();
    }
  }
}

bool isNumber(const std::string& s)
{
  return !s.empty() && std::find_if(s.begin(),
    s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}
