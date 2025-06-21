#include "Data_struct.h"

#include <iomanip>
#include <sstream>

namespace yakovlevart
{
    std::istream& operator>>(std::istream& in, DelimIO&& val) noexcept
    {
        char ch{};
        in >> ch;
        if (!in || ch != val.val) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& val) noexcept
    {
        for (char exp : val.val)
        {
            char got{};
            in >> got;
            if (!in || got != exp) { in.setstate(std::ios::failbit); break; }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& val) noexcept
    {
        char ch{};
        in >> ch;
        if (ch != '"') { in.setstate(std::ios::failbit); return in; }
        val.val.clear();
        while (in.get(ch) && ch != '"') val.val += ch;
        return in >> DelimIO{ ':' };
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& val) noexcept
    {
        std::string buf;
        in >> buf;
        if (buf.empty() || (buf.back() != 'd' && buf.back() != 'D'))
        {
            in.setstate(std::ios::failbit); return in;
        }
        buf.pop_back();
        try { val.val = std::stod(buf); }
        catch (...) { in.setstate(std::ios::failbit); }
        return in >> DelimIO{ ':' };
    }

    std::istream& operator>>(std::istream& in, RationalIO&& val) noexcept
    {
        in >> DelimIO{ '(' } >> DelimIO{ ':' } >> LabelIO{ "N" } >> val.val.first
            >> DelimIO{ ':' } >> LabelIO{ "D" } >> val.val.second
            >> DelimIO{ ':' } >> DelimIO{ ')' } >> DelimIO{ ':' };
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& value) noexcept
    {
        char ch{};
        while (in >> ch) { if (ch == '(') break; }

        DataStruct tmp{};
        bool ok1{}, ok2{}, ok3{};
        in >> DelimIO{ ':' };

        for (int i = 0; i < 3 && in; ++i)
        {
            in >> LabelIO{ "key" };
            size_t num{};
            in >> num;
            switch (num)
            {
            case 1: in >> DoubleIO{ tmp.key1 }; ok1 = in.good(); break;
            case 2: in >> RationalIO{ tmp.key2 }; ok2 = in.good(); break;
            case 3: in >> StringIO{ tmp.key3 }; ok3 = in.good(); break;
            default: in.setstate(std::ios::failbit);
            }
        }
        in >> DelimIO{ ')' };

        if (ok1 && ok2 && ok3) value = tmp;
        else in.setstate(std::ios::failbit);
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& v) noexcept
    {
        out << std::nouppercase << std::fixed << std::setprecision(1);
        out << "(:key1 " << v.key1 << 'd'
            << ":key2 (:N " << v.key2.first << ":D " << v.key2.second << ":)"
            << ":key3 \"" << v.key3 << "\":)";
        return out;
    }

    bool operator<(const DataStruct& l, const DataStruct& r) noexcept
    {
        if (l.key1 != r.key1) return l.key1 < r.key1;
        long double lv = static_cast<long double>(l.key2.first) / l.key2.second;
        long double rv = static_cast<long double>(r.key2.first) / r.key2.second;
        if (lv != rv) return lv < rv;
        return l.key3.size() < r.key3.size();
    }
}
