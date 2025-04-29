#include "Commands.h"

const std::string ERROR_OF_WRONG_COMMAND = "<INVALID COMMAND>";

namespace kirillova
{
  enum class TypeOfCommand
  {
    AREA,
    MAX,
    MIN,
    COUNT,
    LESSAREA,
    SAME,
    INVALID
  };

  TypeOfCommand getTypeOfCommand(const std::string& cmd)
  {
    if (cmd == "AREA") return TypeOfCommand::AREA;
    if (cmd == "MAX") return TypeOfCommand::MAX;
    if (cmd == "MIN") return TypeOfCommand::MIN;
    if (cmd == "COUNT") return TypeOfCommand::COUNT;
    if (cmd == "LESSAREA") return TypeOfCommand::LESSAREA;
    if (cmd == "SAME") return TypeOfCommand::SAME;
    return TypeOfCommand::INVALID;
  }

  bool isValidVertexCount(int vertices)
  {
    return vertices >= 3;
  }

  void executeCommands(const std::vector<Polygon>& polygons, const std::string& command, const std::string& arguments)
  {
    using namespace std::placeholders;

    switch (getTypeOfCommand(command))
    {
      case TypeOfCommand::AREA:
        if (arguments == "EVEN" || arguments == "ODD")
        {
          int parity = (arguments == "EVEN") ? 0 : 1;
          double sum = std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            std::bind(std::plus<>(), _1,
              std::bind(
                std::multiplies<double>(),
                std::bind(
                  std::equal_to<>(),
                  std::bind(std::modulus<int>(), std::bind(getPolygonSize, _2), 2),
                  parity
                ),
                std::bind(getPolygonsArea, _2)
              )
            )
          );
          std::cout << std::fixed << std::setprecision(1) << sum << '\n';
        }
        else if (arguments == "MEAN")
        {
          if (polygons.empty())
          {
            std::cout << ERROR_OF_WRONG_COMMAND << "\n";
            return;
          }

          double total = std::accumulate(
            polygons.begin(), polygons.end(), 0.0,
            std::bind(std::plus<>(), _1, std::bind(getPolygonsArea, _2))
          );
          double mean = total / polygons.size();
          std::cout << std::fixed << std::setprecision(1) << mean << '\n';
        }
        else
        {
          try
          {
            int vertices = std::stoi(arguments);
            if (!isValidVertexCount(vertices))
            {
              std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
              return;
            }

            double sum = std::accumulate(
              polygons.begin(), polygons.end(), 0.0,
              std::bind(std::plus<>(), _1,
                std::bind(
                  std::multiplies<double>(),
                  std::bind(
                    std::equal_to<>(),
                    std::bind(getPolygonSize, _2),
                    vertices
                  ),
                  std::bind(getPolygonsArea, _2)
                )
              )
            );
            std::cout << std::fixed << std::setprecision(1) << sum << '\n';
          }
          catch (...)
          {
            std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
          }
        }
        break;

      case TypeOfCommand::MAX:
        if (polygons.empty())
        {
          std::cout << ERROR_OF_WRONG_COMMAND << "\n";
          return;
        }

        if (arguments == "AREA")
        {
          auto it = std::max_element(
            polygons.begin(), polygons.end(),
            std::bind(std::less<>(), std::bind(getPolygonsArea, _1), std::bind(getPolygonsArea, _2))
          );
          std::cout << std::fixed << std::setprecision(1) << getPolygonsArea(*it) << '\n';
        }
        else if (arguments == "VERTEXES")
        {
          auto it = std::max_element(
            polygons.begin(), polygons.end(),
            std::bind(std::less<>(), std::bind(getPolygonSize, _1), std::bind(getPolygonSize, _2))
          );
          std::cout << getPolygonSize(*it) << '\n';
        }
        else
        {
          std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
        }
        break;

      case TypeOfCommand::MIN:
        if (polygons.empty())
        {
          std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
          return;
        }

        if (arguments == "AREA")
        {
          auto it = std::min_element(
            polygons.begin(), polygons.end(),
            std::bind(std::less<>(), std::bind(getPolygonsArea, _1), std::bind(getPolygonsArea, _2))
          );
          std::cout << std::fixed << std::setprecision(1) << getPolygonsArea(*it) << '\n';
        }
        else if (arguments == "VERTEXES")
        {
          auto it = std::min_element(
            polygons.begin(), polygons.end(),
            std::bind(std::less<>(), std::bind(getPolygonSize, _1), std::bind(getPolygonSize, _2))
          );
          std::cout << getPolygonSize(*it) << '\n';
        }
        else
        {
          std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
        }
        break;

      case TypeOfCommand::COUNT:
        if (arguments == "EVEN" || arguments == "ODD")
        {
          int parity = (arguments == "EVEN") ? 0 : 1;
          auto count = std::count_if(
            polygons.begin(), polygons.end(),
            std::bind(
              std::equal_to<>(),
              std::bind(std::modulus<int>(), std::bind(getPolygonSize, _1), 2),
              parity
            )
          );
          std::cout << count << '\n';
        }
        else
        {
          try
          {
            int vertices = std::stoi(arguments);
            if (!isValidVertexCount(vertices))
            {
              std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
              return;
            }

            auto count = std::count_if(
              polygons.begin(), polygons.end(),
              std::bind(
                std::equal_to<>(),
                std::bind(getPolygonSize, _1),
                vertices
              )
            );
            std::cout << count << '\n';
          }
          catch (...)
          {
            std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
          }
        }
        break;

      case TypeOfCommand::LESSAREA:
        try
        {
          Polygon target;
          std::istringstream iss(arguments);
          iss >> target;

          if (iss.fail() || target.points.size() < 3)
          {
            std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
            return;
          }

          double targetArea = getPolygonsArea(target);
          auto count = std::count_if(
            polygons.begin(), polygons.end(),
            [targetArea](const Polygon& p)
            {
              return getPolygonsArea(p) < targetArea;
            }
          );
          std::cout << count << '\n';
        }
        catch (...)
        {
          std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
        }
        break;

      case TypeOfCommand::SAME:
        try
        {
          Polygon target;
          std::istringstream iss(arguments);
          iss >> target;

          if (iss.fail() || target.points.size() < 3)
          {
            std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
            return;
          }

          auto count = std::count_if(
            polygons.begin(), polygons.end(),
            [&target](const Polygon& p)
            {
              return p.points.size() == target.points.size() &&
                     std::is_permutation(p.points.begin(), p.points.end(), target.points.begin());
            }
          );
          std::cout << count << '\n';
        }
        catch (...)
        {
          std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
        }
        break;

      case TypeOfCommand::INVALID:
      default:
        std::cerr << ERROR_OF_WRONG_COMMAND << "\n";
        break;
    }
  }
}
