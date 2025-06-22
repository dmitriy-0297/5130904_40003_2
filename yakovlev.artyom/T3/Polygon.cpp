#include "Polygon.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <tuple>
#include <stdexcept>

bool Point::operator==(const Point& other) const
{
    return x == other.x && y == other.y;
}

std::istream& operator>>(std::istream& in, Point& pt)
{
    char open{}, sep{}, close{};
    if (in >> open >> pt.x >> sep >> pt.y >> close &&
        open == '(' && sep == ';' && close == ')')
    {
        return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

size_t Polygon::vertexCount() const
{
    return points.size();
}

double Polygon::area() const
{
    if (points.size() < 3)
    {
        return 0.0;
    }
    auto acc = std::inner_product(points.begin(),
        points.end() - 1,
        points.begin() + 1,
        0.0,
        std::plus<>(),
        [](const Point& a, const Point& b)
        { return a.x * b.y - b.x * a.y; });
    acc += points.back().x * points.front().y -
        points.front().x * points.back().y;
    return std::abs(acc) / 2.0;
}

bool Polygon::isRectangle() const
{
    if (vertexCount() != 4)
    {
        return false;
    }
    auto dot = [](const Point& u, const Point& v) { return u.x * v.x + u.y * v.y; };
    for (size_t i = 0; i < 4; ++i)
    {
        const Point& a = points[i];
        const Point& b = points[(i + 1) % 4];
        const Point& c = points[(i + 2) % 4];
        Point ab{ b.x - a.x, b.y - a.y };
        Point bc{ c.x - b.x, c.y - b.y };
        if (dot(ab, bc) != 0)
        {
            return false;
        }
    }
    return true;
}

bool Polygon::isEven() const
{
    return vertexCount() % 2 == 0;
}

bool Polygon::hasVertexCount(size_t n) const
{
    return vertexCount() == n;
}

bool Polygon::hasRightAngle() const
{
    if (vertexCount() < 3)
    {
        return false;
    }
    auto dot = [](const Point& u, const Point& v) { return u.x * v.x + u.y * v.y; };

    std::vector<size_t> idx(vertexCount());
    std::iota(idx.begin(), idx.end(), 0);

    return std::any_of(idx.begin(), idx.end(),
        [&](size_t i)
        {
            const Point& a = points[i];
            const Point& b = points[(i + 1) % vertexCount()];
            const Point& c = points[(i + 2) % vertexCount()];
            Point ab{ b.x - a.x, b.y - a.y };
            Point bc{ c.x - b.x, c.y - b.y };
            return dot(ab, bc) == 0;
        });
}

bool Polygon::operator==(const Polygon& other) const
{
    if (points.size() != other.points.size())
    {
        return false;
    }
    auto normalize = [](const std::vector<Point>& pts)
        {
            auto minPt = *std::min_element(
                pts.begin(), pts.end(),
                [](const Point& a, const Point& b)
                { return std::tie(a.x, a.y) < std::tie(b.x, b.y); });
            std::vector<Point> res;
            res.reserve(pts.size());
            std::transform(pts.begin(), pts.end(), std::back_inserter(res),
                [&](const Point& p) { return Point{ p.x - minPt.x, p.y - minPt.y }; });
            return res;
        };
    return normalize(points) == normalize(other.points);
}

std::istream& operator>>(std::istream& in, Polygon& p)
{
    p.points.clear();
    size_t n{};
    if (!(in >> n) || n < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::vector<Point> tmp;
    tmp.reserve(n);
    std::copy_n(std::istream_iterator<Point>(in), n, std::back_inserter(tmp));

    if (tmp.size() != n)
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    p.points = std::move(tmp);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Polygon& p)
{
    out << p.points.size();
    for (const auto& pt : p.points)
    {
        out << " (" << pt.x << ';' << pt.y << ')';
    }
    return out;
}
