#include "Data_struct.h"

#include <iomanip>
#include <sstream>
#include <cmath>

namespace yakovlevart
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& val) noexcept
    {
        char ch{};
        in >> ch;
        if (!in || ch != val.val)
            in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& val) noexcept
    {
        for (char expected : val.val)
        {
            char actual{};
            in >> actual;
            if (!in || actual != expected)
            {
                in.setstate(std::ios::failbit);
                break;
            }
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& val) noexcept
    {
        char ch{};
        in >> ch;
        if (ch != '"')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        val.val.clear();
        while (in.get(ch))
        {
            if (ch == '"')
                break;
            val.val += ch;
        }
        return in >> DelimiterIO{ ':' };
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& val) noexcept
    {
        std::string num;
        in >> num;
        if (num.empty() || (num.back() != 'd' && num.back() != 'D'))
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        num.pop_back();
        try
        {
            val.val = std::stod(num);
        }
        catch (...)
        {
            in.setstate(std::ios::failbit);
        }
        return in >> DelimiterIO{ ':' };
    }

    std::istream& operator>>(std::istream& in, RationalIO&& val) noexcept
    {
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> LabelIO{ "N" } >> val.val.first
            >> DelimiterIO{ ':' } >> LabelIO{ "D" } >> val.val.second
            >> DelimiterIO{ ':' } >> DelimiterIO{ ')' } >> DelimiterIO{ ':' };
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& value) noexcept
    {
        char ch{};
        while (in >> ch)
        {
            if (ch == '(')
                break;
        }

        DataStruct tmp{};
        bool ok1 = false, ok2 = false, ok3 = false;

        in >> DelimiterIO{ ':' };
        while (in && in.peek() != ')')
        {
            in >> LabelIO{ "key" };
            size_t num{};
            in >> num;

            switch (num)
            {
            case 1: in >> DoubleIO{ tmp.key1 };         ok1 = in.good(); break;
            case 2: in >> RationalIO{ tmp.key2 };       ok2 = in.good(); break;
            case 3: in >> StringIO{ tmp.key3 };         ok3 = in.good(); break;
            default: in.setstate(std::ios::failbit);    break;
            }
        }

        in >> DelimiterIO{ ')' };

        if (ok1 && ok2 && ok3)
        {
            value = tmp;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& value) noexcept
    {
        out << std::nouppercase << std::fixed << std::setprecision(1);
        out << "(:key1 " << value.key1 << 'd'
            << ":key2 (:N " << value.key2.first << ":D " << value.key2.second << ":)"
            << ":key3 \"" << value.key3 << "\":)";
        return out;
    }

    bool operator<(const DataStruct& lhs, const DataStruct& rhs) noexcept
    {
        if (lhs.key1 != rhs.key1)
            return lhs.key1 < rhs.key1;

        const long double left = static_cast<long double>(lhs.key2.first) / lhs.key2.second;
        const long double right = static_cast<long double>(rhs.key2.first) / rhs.key2.second;
        if (left != right)
            return left < right;

        return lhs.key3.length() < rhs.key3.length();
    }
}
