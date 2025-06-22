#include "Commands.h"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <sstream>
#include <stdexcept>

namespace
{
    struct BoundingBox
    {
        int minX{ std::numeric_limits<int>::max() };
        int maxX{ std::numeric_limits<int>::min() };
        int minY{ std::numeric_limits<int>::max() };
        int maxY{ std::numeric_limits<int>::min() };
    };

    BoundingBox calcBounds(const std::vector<Polygon>& polys)
    {
        BoundingBox box;
        for (const auto& poly : polys)
        {
            for (const auto& pt : poly.points)
            {
                box.minX = std::min(box.minX, pt.x);
                box.maxX = std::max(box.maxX, pt.x);
                box.minY = std::min(box.minY, pt.y);
                box.maxY = std::max(box.maxY, pt.y);
            }
        }
        return box;
    }
}

void executeCommands(const std::vector<Polygon>& polygons)
{
    std::cout << std::fixed << std::setprecision(1);
    const BoundingBox globalBox = calcBounds(polygons);
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
        {
            continue;
        }
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        try
        {
            if (cmd == "AREA")
            {
                std::string arg;
                iss >> arg;
                double sum = 0.0;
                if (arg == "EVEN" || arg == "ODD")
                {
                    bool even = arg == "EVEN";
                    sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [even](double a, const Polygon& p)
                        { return (p.isEven() == even) ? a + p.area() : a; });
                }
                else if (arg == "MEAN")
                {
                    if (polygons.empty())
                    {
                        throw std::invalid_argument("no polygons");
                    }
                    sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double a, const Polygon& p) { return a + p.area(); });
                    sum /= polygons.size();
                }
                else
                {
                    size_t v = std::stoul(arg);
                    if (v < 3)
                    {
                        throw std::invalid_argument("bad v");
                    }
                    sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [v](double a, const Polygon& p)
                        { return p.hasVertexCount(v) ? a + p.area() : a; });
                }
                std::cout << sum << '\n';
            }
            else if (cmd == "MAX")
            {
                std::string arg;
                iss >> arg;
                if (polygons.empty())
                {
                    throw std::invalid_argument("empty");
                }
                if (arg == "AREA")
                {
                    auto it = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b)
                        { return a.area() < b.area(); });
                    std::cout << it->area() << '\n';
                }
                else if (arg == "VERTEXES")
                {
                    auto it = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b)
                        { return a.vertexCount() < b.vertexCount(); });
                    std::cout << it->vertexCount() << '\n';
                }
                else
                {
                    throw std::invalid_argument("bad MAX arg");
                }
            }
            else if (cmd == "MIN")
            {
                std::string arg;
                iss >> arg;
                if (polygons.empty())
                {
                    throw std::invalid_argument("empty");
                }
                if (arg == "AREA")
                {
                    auto it = std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b)
                        { return a.area() < b.area(); });
                    std::cout << it->area() << '\n';
                }
                else if (arg == "VERTEXES")
                {
                    auto it = std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b)
                        { return a.vertexCount() < b.vertexCount(); });
                    std::cout << it->vertexCount() << '\n';
                }
                else
                {
                    throw std::invalid_argument("bad MIN arg");
                }
            }
            else if (cmd == "COUNT")
            {
                std::string arg;
                iss >> arg;
                size_t cnt = 0;
                if (arg == "EVEN" || arg == "ODD")
                {
                    bool even = arg == "EVEN";
                    cnt = std::count_if(polygons.begin(), polygons.end(),
                        [even](const Polygon& p) { return p.isEven() == even; });
                }
                else
                {
                    size_t v = std::stoul(arg);
                    if (v < 3)
                    {
                        throw std::invalid_argument("bad COUNT arg");
                    }
                    cnt = std::count_if(polygons.begin(), polygons.end(),
                        [v](const Polygon& p) { return p.hasVertexCount(v); });
                }
                std::cout << cnt << '\n';
            }
            else if (cmd == "RECTS")
            {
                size_t cnt = std::count_if(polygons.begin(), polygons.end(),
                    std::mem_fn(&Polygon::isRectangle));
                std::cout << cnt << '\n';
            }
            else if (cmd == "SAME")
            {
                Polygon target;
                if (!(iss >> target))
                {
                    throw std::invalid_argument("bad SAME");
                }
                size_t cnt = std::count(polygons.begin(), polygons.end(), target);
                std::cout << cnt << '\n';
            }
            else if (cmd == "RIGHTSHAPES")
            {
                size_t cnt = std::count_if(polygons.begin(), polygons.end(),
                    std::mem_fn(&Polygon::hasRightAngle));
                std::cout << cnt << '\n';
            }
            else if (cmd == "INFRAME")
            {
                Polygon test;
                if (!(iss >> test))
                {
                    throw std::invalid_argument("bad INFRAME");
                }
                bool inside = std::all_of(test.points.begin(), test.points.end(),
                    [&](const Point& pt)
                    {
                        return globalBox.minX <= pt.x && pt.x <= globalBox.maxX &&
                            globalBox.minY <= pt.y && pt.y <= globalBox.maxY;
                    });
                std::cout << (inside ? "<TRUE>" : "<FALSE>") << '\n';
            }
            else
            {
                throw std::invalid_argument("unknown");
            }
        }
        catch (const std::exception&)
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}
