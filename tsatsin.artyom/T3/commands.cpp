#include "commands.h"
#include <string>
#include <algorithm>

namespace artemonts
{
    static void invalid(std::ostream& out)
    {
        out << "<INVALID COMMAND>\n";
    }

    static bool isValidVertexCount(size_t n)
    {
        return n >= 3;
    }

    static void doAreaCommand(const std::vector<Polygon>& polys,
        std::istream& in, std::ostream& out)
    {
        std::string arg;
        in >> arg;
        if (!in)
        {
            invalid(out);
            return;
        }
        if (arg == "EVEN" || arg == "ODD")
        {
            bool odd = (arg == "ODD");
            double res = 0.0;
            for (const auto& p : polys)
                if ((p.points.size() % 2 == 1) == odd)
                    res += area(p);
            out << res << '\n';
            return;
        }
        if (arg == "MEAN")
        {
            if (polys.empty())
            {
                invalid(out);
                return;
            }
            double sum = 0.0;
            for (const auto& p : polys)
                sum += area(p);
            out << sum / polys.size() << '\n';
            return;
        }
        try
        {
            size_t n = std::stoul(arg);
            if (!isValidVertexCount(n))
            {
                invalid(out);
                return;
            }
            double sum = 0.0;
            for (const auto& p : polys)
                if (p.points.size() == n)
                    sum += area(p);
            out << sum << '\n';
        }
        catch (...)
        {
            invalid(out);
        }
    }

    template<class CmpArea, class CmpVert>
    static void doExtremum(const std::vector<Polygon>& polys,
        std::istream& in, std::ostream& out,
        CmpArea cmpArea, CmpVert cmpV)
    {
        std::string arg;
        in >> arg;
        if (!in || polys.empty())
        {
            invalid(out);
            return;
        }
        if (arg == "AREA")
        {
            double ex = area(polys.front());
            for (auto& p : polys)
                if (cmpArea(area(p), ex))
                    ex = area(p);
            out << ex << '\n';
        }
        else if (arg == "VERTEXES")
        {
            size_t ex = polys.front().points.size();
            for (auto& p : polys)
                if (cmpV(p.points.size(), ex))
                    ex = p.points.size();
            out << ex << '\n';
        }
        else
        {
            invalid(out);
        }
    }

    static void doMaxCommand(const std::vector<Polygon>& polys,
        std::istream& in, std::ostream& out)
    {
        doExtremum(polys, in, out,
            std::greater<double>(), std::greater<size_t>());
    }

    static void doMinCommand(const std::vector<Polygon>& polys,
        std::istream& in, std::ostream& out)
    {
        doExtremum(polys, in, out,
            std::less<double>(), std::less<size_t>());
    }

    static void doCountCommand(const std::vector<Polygon>& polys,
        std::istream& in, std::ostream& out)
    {
        std::string arg;
        in >> arg;
        if (!in)
        {
            invalid(out);
            return;
        }
        if (arg == "EVEN" || arg == "ODD")
        {
            bool odd = (arg == "ODD");
            size_t cnt = 0;
            for (auto& p : polys)
                if ((p.points.size() % 2 == 1) == odd)
                    ++cnt;
            out << cnt << '\n';
            return;
        }
        try
        {
            size_t n = std::stoul(arg);
            if (n < 3)
            {
                invalid(out);
                return;
            }
            size_t cnt = 0;
            for (auto& p : polys)
                if (p.points.size() == n)
                    ++cnt;
            out << cnt << '\n';
        }
        catch (...)
        {
            invalid(out);
        }
    }

    static void doRectsCommand(const std::vector<Polygon>& polys,
        std::istream&, std::ostream& out)
    {
        size_t cnt = 0;
        for (auto& p : polys)
            if (isRectangle(p))
                ++cnt;
        out << cnt << '\n';
    }

    static void doMaxSeqCommand(const std::vector<Polygon>& polys,
        std::istream& in, std::ostream& out)
    {
        Polygon target;
        in >> target;
        if (target.points.size() < 3)
        {
            invalid(out);
            return;
        }
        size_t best = 0;
        size_t cur = 0;
        for (auto& p : polys)
            if (p == target)
            {
                ++cur;
                best = std::max(best, cur);
            }
            else
            {
                cur = 0;
            }
        out << best << '\n';
    }

    std::map<std::string, std::function<void(std::istream&, std::ostream&)>>
        makeCommands(const std::vector<Polygon>& polys)
    {
        using namespace std::placeholders;
        return {
          { "AREA",   std::bind(doAreaCommand,  std::cref(polys), _1, _2) },
          { "MAX",    std::bind(doMaxCommand,   std::cref(polys), _1, _2) },
          { "MIN",    std::bind(doMinCommand,   std::cref(polys), _1, _2) },
          { "COUNT",  std::bind(doCountCommand, std::cref(polys), _1, _2) },
          { "RECTS",  std::bind(doRectsCommand, std::cref(polys), _1, _2) },
          { "MAXSEQ", std::bind(doMaxSeqCommand,std::cref(polys), _1, _2) }
        };
    }
}
