#include "Data_struct.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cmath>

namespace yakovlevart
{
    constexpr double EPSILON = 1e-6;

    std::istream& operator>>(std::istream& in, DelimiterIO&& v)
    {
        char c = 0;
        in >> c;
        if (in && c != v.val) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& v)
    {
        std::string label;
        in >> label;
        if (in && label != v.val) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& v)
    {
        char c = 0;
        in >> c;
        if (c != '"') {
            in.setstate(std::ios::failbit);
            return in;
        }
        v.val.clear();
        while (in.get(c) && c != '"') {
            v.val += c;
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& v)
    {
        std::string str;
        in >> str;
        if (str.empty()) {
            in.setstate(std::ios::failbit);
            return in;
        }
        if (!str.empty() && (str.back() == 'd' || str.back() == 'D' || str.back() == 'e' || str.back() == 'E')) {
            str.pop_back();
        }
        try {
            size_t pos = 0;
            v.val = std::stod(str, &pos);
            if (pos != str.size()) {
                in.setstate(std::ios::failbit);
            }
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& v)
    {
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> LabelIO{ "N" } >> v.val.first
            >> DelimiterIO{ ':' } >> LabelIO{ "D" } >> v.val.second
            >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& value)
    {
        char c = 0;
        in >> c;
        if (c != '(') {
            in.setstate(std::ios::failbit);
            return in;
        }

        DataStruct tmp;
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;

        while (true) {
            while (in.peek() == ' ') in.get();

            if (in.peek() == ')') {
                in.get();
                break;
            }

            if (!(in >> DelimiterIO{ ':' } >> LabelIO{ "key" })) {
                in.setstate(std::ios::failbit);
                break;
            }

            size_t num = 0;
            in >> num;

            switch (num) {
            case 1:
                if (hasKey1) {
                    in.setstate(std::ios::failbit);
                }
                else {
                    in >> DoubleIO{ tmp.key1 };
                    hasKey1 = !in.fail();
                }
                break;
            case 2:
                if (hasKey2) {
                    in.setstate(std::ios::failbit);
                }
                else {
                    in >> RationalIO{ tmp.key2 };
                    hasKey2 = !in.fail();
                }
                break;
            case 3:
                if (hasKey3) {
                    in.setstate(std::ios::failbit);
                }
                else {
                    in >> StringIO{ tmp.key3 };
                    hasKey3 = !in.fail();
                }
                break;
            default:
                in.setstate(std::ios::failbit);
            }

            if (in.fail()) {
                break;
            }
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            value = tmp;
        }
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& v)
    {
        out << "(:key1 " << std::fixed << std::setprecision(1)
            << v.key1 << 'd';
        out << ":key2 (:N " << v.key2.first << ":D "
            << v.key2.second << ":)";
        out << ":key3 \"" << v.key3 << "\":)";
        return out;
    }

    bool operator<(const DataStruct& a, const DataStruct& b)
    {
        if (std::abs(a.key1 - b.key1) > EPSILON) {
            return a.key1 < b.key1;
        }

        const long double lhs = static_cast<long double>(a.key2.first) / a.key2.second;
        const long double rhs = static_cast<long double>(b.key2.first) / b.key2.second;
        if (std::abs(lhs - rhs) > EPSILON) {
            return lhs < rhs;
        }

        return a.key3.length() < b.key3.length();
    }
}
