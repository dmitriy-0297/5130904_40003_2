#include "geometry.h"
#include <cmath>

namespace artemonts
{
    std::istream& operator>>(std::istream& in, Point& p)
    {
        char c1{}, c2{}, c3{};
        in >> c1 >> p.x >> c2 >> p.y >> c3;
        if (c1 != '(' || c2 != ';' || c3 != ')')
            in.setstate(std::ios::failbit);
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const Point& p)
    {
        return out << '(' << p.x << ';' << p.y << ')';
    }

    bool operator==(const Point& a, const Point& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    std::istream& operator>>(std::istream& in, Polygon& poly)
    {
        size_t n = 0;
        in >> n;
        poly.points.clear();
        poly.points.reserve(n);
        for (size_t i = 0; i < n && in; ++i)
        {
            Point pt;
            in >> pt;
            poly.points.push_back(pt);
        }
        return in;
    }

    bool operator==(const Polygon& a, const Polygon& b)
    {
        return a.points == b.points;
    }

    static long long cross(const Point& a, const Point& b)
    {
        return static_cast<long long>(a.x) * b.y
            - static_cast<long long>(b.x) * a.y;
    }

    double area(const Polygon& poly)
    {
        if (poly.points.size() < 3)
            return 0.0;
        long long sum = 0;
        for (size_t i = 0; i + 1 < poly.points.size(); ++i)
            sum += cross(poly.points[i], poly.points[i + 1]);
        sum += cross(poly.points.back(), poly.points.front());
        return std::abs(sum) / 2.0;
    }

    static bool isRightAngle(const Point& a, const Point& b, const Point& c)
    {
        long long abx = a.x - b.x;
        long long aby = a.y - b.y;
        long long cbx = c.x - b.x;
        long long cby = c.y - b.y;
        return abx * cbx + aby * cby == 0;
    }

    bool isRectangle(const Polygon& p)
    {
        if (p.points.size() != 4)
            return false;
        for (size_t i = 0; i < 4; ++i)
        {
            const Point& prev = p.points[(i + 3) % 4];
            const Point& cur = p.points[i];
            const Point& next = p.points[(i + 1) % 4];
            if (!isRightAngle(prev, cur, next))
                return false;
        }
        return true;
    }
}
