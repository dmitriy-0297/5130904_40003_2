#include "Data_struct.h"
#include <iomanip>
#include <limits>
#include <sstream>

namespace yakovlev
{

    std::istream& operator>>(std::istream& in, Del d) noexcept
    {
        char c{};
        in >> c;
        if (!in || c != d.ch) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, Lbl l) noexcept
    {
        for (const char* p = l.literal; *p && in; ++p)
        {
            char c{};
            in >> c;
            if (c != *p) in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, Str s) noexcept
    {
        in >> Del{ '"' };
        std::getline(in, s.v, '"');
        return in >> Del{ ':' };
    }

    std::istream& operator>>(std::istream& in, Dbl d) noexcept
    {
        std::string tok;
        in >> tok;
        if (tok.size() < 2 || (tok.back() != 'd' && tok.back() != 'D'))
            return in.setstate(std::ios::failbit), in;

        tok.pop_back();
        try { d.v = std::stod(tok); }
        catch (...) { in.setstate(std::ios::failbit); }
        return in >> Del{ ':' };
    }

    std::istream& operator>>(std::istream& in, Rat r) noexcept
    {
        return in >> Del{ '(' } >> Del{ ':' } >> Lbl{ "N" } >> r.v.first
            >> Del{ ':' } >> Lbl{ "D" } >> r.v.second
            >> Del{ ':' } >> Del{ ')' } >> Del{ ':' };
    }


    std::istream& operator>>(std::istream& in, DataStruct& dst) noexcept
    {
        char c{};
        while (in.get(c) && c != '(') {}
        if (!in) return in;

        DataStruct tmp{};
        bool ok1 = false, ok2 = false, ok3 = false;

        in >> Del{ ':' };

        for (int i = 0; i < 3 && in; ++i)
        {
            in >> Lbl{ "key" };
            std::size_t id{};
            in >> id;

            switch (id)
            {
            case 1: in >> Dbl{ tmp.key1 };                              ok1 = in.good(); break;
            case 2: in >> Rat{ tmp.key2 };                              ok2 = in.good(); break;
            case 3: in >> Str{ tmp.key3 };                              ok3 = in.good(); break;
            default: in.setstate(std::ios::failbit);                                break;
            }
        }
        in >> Del{ ')' };

        if (ok1 && ok2 && ok3) dst = tmp;
        else                   in.setstate(std::ios::failbit);
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& d) noexcept
    {
        out << std::fixed << std::setprecision(1) << std::nouppercase;
        out << "(:key1 " << d.key1 << 'd'
            << ":key2 (:N " << d.key2.first
            << ":D " << d.key2.second << ":)"
            << ":key3 \"" << d.key3 << "\":)";
        return out;
    }

    bool operator<(const DataStruct& l, const DataStruct& r) noexcept
    {
        if (l.key1 != r.key1) return l.key1 < r.key1;

        long double ql = static_cast<long double>(l.key2.first) / l.key2.second;
        long double qr = static_cast<long double>(r.key2.first) / r.key2.second;
        if (ql != qr)   return ql < qr;

        return l.key3.size() < r.key3.size();
    }
}
