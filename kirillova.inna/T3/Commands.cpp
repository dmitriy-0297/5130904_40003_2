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
                        std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                catch (const std::invalid_argument& except)
                {
                    std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                std::cout << ERROR_OF_WRONG_COMMAND << "\n";
            }
            break;

        case TypeOfCommand::MIN:
            if (polygons.empty())
            {
                std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                        std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                catch (const std::invalid_argument& except)
                {
                    std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                    std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
            catch (const std::invalid_argument& except)
            {
                std::cout << ERROR_OF_WRONG_COMMAND << "\n";
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
                    std::cout << ERROR_OF_WRONG_COMMAND << "\n";
                    return;
                }

                auto count = std::count_if(
                    polygons.begin(), polygons.end(),
                    [&target](const Polygon& p)
                    {
                        if (p.points.size() < 3)
                            return false;

                        const auto& A = target.points;
                        const auto& B = p.points;
                        if (A.size() != B.size())
                            return false;

                        auto is_match = [&A, &B](size_t offset)
                            {
                                return std::equal(
                                    A.begin(), A.end(),
                                    B.begin() + offset,
                                    [](const Point& p1, const Point& p2)
                                    {
                                        return p1 == p2;
                                    }
                                );
                            };

                        return std::any_of(
                            B.begin(), B.end(),
                            std::bind(is_match, std::placeholders::_1)
                        );
                    }
                );

                std::cout << count << '\n';
            }
            catch (const std::invalid_argument& except)
            {
                std::cout << ERROR_OF_WRONG_COMMAND << "\n";
            }
            break;

        case TypeOfCommand::INVALID:
        default:
            std::cout << ERROR_OF_WRONG_COMMAND << "\n";
            break;
        }
    }
}
