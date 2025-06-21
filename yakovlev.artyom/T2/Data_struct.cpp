#include "Data_struct.h"
#include <iomanip>
#include <sstream>

namespace yakovlevart
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& v) noexcept
    {
        char c{}; in >> c; if (!in || c != v.val) in.setstate(std::ios::failbit);
        return in;
    }
    std::istream& operator>>(std::istream& in, LabelIO&& v) noexcept
    {
        for (char e : v.val) { char a{}; in >> a; if (!in || a != e) { in.setstate(std::ios::failbit); break; } }
        return in;
    }
    std::istream& operator>>(std::istream& in, StringIO&& v) noexcept
    {
        char c{}; in >> c; if (c != '"') { in.setstate(std::ios::failbit); return in; }
        v.val.clear(); while (in.get(c)) { if (c == '"') break; v.val += c; }
        return in >> DelimiterIO{ ':' };
    }
    std::istream& operator>>(std::istream& in, DoubleIO&& v) noexcept
    {
        std::string s; in >> s;
        if (s.empty() || (s.back() != 'd' && s.back() != 'D')) { in.setstate(std::ios::failbit); return in; }
        s.pop_back(); try { v.val = std::stod(s); }
        catch (...) { in.setstate(std::ios::failbit); }
        return in >> DelimiterIO{ ':' };
    }
    std::istream& operator>>(std::istream& in, RationalIO&& v) noexcept
    {
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> LabelIO{ "N" } >> v.val.first
            >> DelimiterIO{ ':' } >> LabelIO{ "D" } >> v.val.second
            >> DelimiterIO{ ':' } >> DelimiterIO{ ')' } >> DelimiterIO{ ':' };
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& value) noexcept
    {
        char c{};
        while (in >> c) { if (c == '(') break; }
        if (!in) return in;

        DataStruct tmp{};
        bool ok1{}, ok2{}, ok3{};
        in >> DelimiterIO{ ':' };

        while (in)
        {
            while (std::isspace(in.peek())) in.get();
            if (in.peek() == ')') break;

            in >> LabelIO{ "key" };
            size_t num{}; in >> num;
            switch (num)
            {
            case 1: in >> DoubleIO{ tmp.key1 };   ok1 = in.good(); break;
            case 2: in >> RationalIO{ tmp.key2 }; ok2 = in.good(); break;
            case 3: in >> StringIO{ tmp.key3 };   ok3 = in.good(); break;
            default: in.setstate(std::ios::failbit);  break;
            }
        }
        in >> DelimiterIO{ ')' };

        if (ok1 && ok2 && ok3) value = tmp;
        else in.setstate(std::ios::fail bit);
        return in;
    }


    std::ostream& operator<<(std::ostream& out, const DataStruct& v) noexcept
    {
        out << std::nouppercase << std::fixed << std::setprecision(1)
            << "(:key1 " << v.key1 << 'd'
            << ":key2 (:N " << v.key2.first << ":D " << v.key2.second << ":)"
            << ":key3 \"" << v.key3 << "\":)";
        return out;
    }

    bool operator<(const DataStruct& a, const DataStruct& b) noexcept
    {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        long double l = static_cast<long double>(a.key2.first) / a.key2.second;
        long double r = static_cast<long double>(b.key2.first) / b.key2.second;
        if (l != r) return l < r;
        return a.key3.length() < b.key3.length();
    }
}
