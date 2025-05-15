#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <numeric>
#include "Polygon.h"
void executeCommands(const std::vector<Polygon>& polygons);
std::vector<Polygon> readPolygonsFromFile(const std::string& filename);
int main(int argc, char* argv[])
{
  try
  {
  if (argc != 2)
  {
    std::cerr << "Error: filename not provided\n";
    return EXIT_FAILURE;
  }
  std::string filename = argv[1];

  std::vector<Polygon> polygons = readPolygonsFromFile(filename);

  executeCommands(polygons);
  }
  catch (const std::runtime_error& e)
  {
  std::cerr << e.what() << '\n';
  return EXIT_FAILURE;
  }
  catch (const std::invalid_argument& e)
  {
  std::cerr << e.what() << '\n';
  return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

std::vector<Polygon> readPolygonsFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Error: cannot open file");
  }

  std::vector<Polygon> polygons;
  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    Polygon p;
    try
    {
      iss >> p;
      if (iss)
      {
        polygons.push_back(p);
      }
    }
    catch (const std::invalid_argument&)
    {
      continue;
    }
  }
  return polygons;
}

void executeCommands(const std::vector<Polygon>& polygons)
{
  std::cout << std::fixed << std::setprecision(1);

  while (std::cin)
  {
    std::string cmd;
    if (!std::getline(std::cin, cmd)) break;
    if (cmd.empty()) continue;

    std::istringstream iss(cmd);
    std::string cmd_name;
    iss >> cmd_name;

    try
    {
      if (cmd_name == "AREA")
      {
        std::string param;
        iss >> param;
        double sum = 0.0;
        size_t count = 0;
        if (param == "EVEN")
        {
          sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p)
            {
              return p.isEven() ? acc + p.area() : acc;
            });
        }
        else if (param == "ODD")
        {
          sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p)
            {
              return !p.isEven() ? acc + p.area() : acc;
            });
        }
        else if (param == "MEAN")
        {
          sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p)
            {
              return acc + p.area();
            });
          count = polygons.size();
          if (count == 0)
            throw std::invalid_argument("No polygons");
          sum /= count;
        }
        else
        {
          try
          {
            size_t num = std::stoi(param);
            sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
              [num](double acc, const Polygon& p)
              {
                return p.hasVertexCount(num) ? acc + p.area() : acc;
              });
          }
          catch (...)
          {
            throw std::invalid_argument("Invalid parameter");
          }
        }
        std::cout << sum << '\n';
      }
      else if (cmd_name == "MAX")
      {
        std::string type;
        iss >> type;
        if (polygons.empty())
          throw std::invalid_argument("No polygons");
        if (type == "AREA")
        {
          auto max_it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b)
            {
              return a.area() < b.area();
            });
          std::cout << (*max_it).area() << '\n';
        }
        else if (type == "VERTEXES")
        {
          auto max_it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b)
            {
              return a.vertexCount() < b.vertexCount();
            });
          std::cout << (*max_it).vertexCount() << '\n';
        }
        else
        {
          throw std::invalid_argument("Invalid parameter");
        }
      }
      else if (cmd_name == "MIN")
      {
        std::string type;
        iss >> type;
        if (polygons.empty())
          throw std::invalid_argument("No polygons");
        if (type == "AREA")
        {
          auto min_it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b)
            {
              return a.area() < b.area();
            });
          std::cout << (*min_it).area() << '\n';
        }
        else if (type == "VERTEXES")
        {
          auto min_it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b)
            {
              return a.vertexCount() < b.vertexCount();
            });
          std::cout << (*min_it).vertexCount() << '\n';
        }
        else
        {
          throw std::invalid_argument("Invalid parameter");
        }
      }
      else if (cmd_name == "COUNT")
      {
        std::string param;
        iss >> param;
        size_t count = 0;
        if (param == "EVEN")
        {
          count = std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p)
            {
              return p.isEven();
            });
        }
        else if (param == "ODD")
        {
          count = std::count_if(polygons.begin(), polygons.end(),
            [](const Polygon& p)
            {
              return !p.isEven();
            });
        }
        else
        {
          try
          {
            size_t num = std::stoi(param);
            count = std::count_if(polygons.begin(), polygons.end(),
              [num](const Polygon& p)
              {
                return p.hasVertexCount(num);
              });
          }
          catch (...)
          {
            throw std::invalid_argument("Invalid parameter");
          }
        }
        std::cout << count << '\n';
      }
      else if (cmd_name == "RECTS")
      {
        size_t count = std::count_if(polygons.begin(), polygons.end(),
          [](const Polygon& p)
          {
            return p.isRectangle();
          });
        std::cout << count << '\n';
      }
      else if (cmd_name == "SAME")
      {
        Polygon target;
        std::istringstream cmd_iss(cmd);
        std::string dummy;
        cmd_iss >> dummy;
        if (!(cmd_iss >> target))
        {
          throw std::invalid_argument("Invalid polygon format");
        }
        size_t same_count = std::count(polygons.begin(), polygons.end(), target);
        std::cout << same_count << '\n';
      }
      else
      {
        throw std::invalid_argument("Unknown command");
      }
    }
    catch (const std::invalid_argument&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}