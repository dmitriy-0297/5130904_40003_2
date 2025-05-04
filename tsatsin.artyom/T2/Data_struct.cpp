#include "Data_struct.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <regex>

namespace artemonts {
    std::istream& operator>>(std::istream& in, StringIO&& val)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c;
        in >> c;
        if (c != '"')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        val.val.clear();
        while (in.get(c))
        {
            if (c == '"')
            {
                break;
            }
            val.val += c;
        }
        if (c != '"')
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        in >> DelimiterIO{ ':' };
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& value)
    {
        char c;

        // Бесконечный цикл: ищем первую скобку '(' или выходим по EOF
        while (true) {
            in >> std::ws;
            if (!in || in.peek() == EOF) {
                in.setstate(std::ios::failbit);
                return in;
            }
            if (in.peek() == '(') break;
            in.get();
        }

        DataStruct tmp;
        bool valid = true;

        // Пытаемся распарсить запись целиком
        if (!(in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' })) valid = false;

        for (int i = 0; i < 3 && valid; ++i) {
            size_t num;
            if (!(in >> LabelIO{ "key" } >> num)) {
                valid = false;
            }
            else if (num == 1) {
                if (!(in >> DoubleIO{ tmp.key1 })) valid = false;
            }
            else if (num == 2) {
                if (!(in >> UnsignedllIO{ tmp.key2 })) valid = false;
            }
            else if (num == 3) {
                if (!(in >> StringIO{ tmp.key3 })) valid = false;
            }
            else {
                valid = false;
            }
        }

        // Должны встретить ')'
        if (!(in >> DelimiterIO{ ')' })) valid = false;

        if (valid) {
            // Всё хорошо — выдаём пользователю
            value = tmp;
            return in;
        }

        // Что‑то пошло не так — очищаем флаг ошибки,
        // пропускаем оставшуюся кусок до ')' и пробуем снова:
        in.clear(in.rdstate() & ~std::ios::failbit);
        while (in.get(c) && c != ')') { /* пропускаем */ }

        // Рекурсивно читаем следующую запись (или уйдём в EOF)
        return operator>>(in, value);
    }


    std::ostream& operator<<(std::ostream& out, const DataStruct& value)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        out << "(";
        out << ":key1 " << fromDoubleToScientific(value.key1);
        out << ":key2 0b" << (value.key2 == 0 ? "" : "0") << fromULLtoBinary(value.key2);
        out << ":key3 \"" << value.key3;
        out << "\":)";
        return out;
    }

    bool operator<(const DataStruct& left, const DataStruct& right)
    {
        if (left.key1 != right.key1)
        {
            return left.key1 < right.key1;
        }
        else if (left.key2 != right.key2)
        {
            return left.key2 < right.key2;
        }
        else
        {
            return left.key3.length() < right.key3.length();
        }
    }

    void reverse_print(const std::vector<DataStruct>& data) {
        std::for_each(data.rbegin(), data.rend(),
            [](const DataStruct& ds) { std::cout << ds << "\n"; });
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& val)
    {
        std::istream::sentry guard(in);
        if (!guard)
        {
            return in;
        }
        char ch = 0;
        in >> ch;
        if (in && ch != std::tolower(val.val) && ch != std::toupper(val.val))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& val)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        int mantissa = 0;
        int number = 0;
        int power = 0;
        in >> mantissa >> DelimiterIO{ '.' } >> number >> DelimiterIO{ 'E' } >> power;
        val.val = (mantissa * 1.0 + number * 0.01) * std::pow(10, power);
        in >> DelimiterIO{ ':' };
        return in;
    }


    std::istream& operator>>(std::istream& in, UnsignedllIO&& val)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        unsigned long long number = 0;
        char c = 0;
        in >> LabelIO{ "0b" };
        while (in >> c)
        {
            if (c == '0' || c == '1')
            {
                number = (number << 1) + (c - '0');
            }
            else
            {
                break;
            }
        }
        if (c == ':')
        {
            val.val = number;
        }
        else
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& val)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        for (size_t i = 0; i < val.val.length(); i++)
        {
            in >> DelimiterIO{ val.val[i] };
        }
        return in;
    }

    std::string fromULLtoBinary(unsigned long long value) {
        if (value == 0) {
            return "0";
        }
        std::string binary;
        while (value > 0) {
            binary.insert(0, std::to_string(value % 2));
            value /= 2;
        }
        return binary;
    }

    std::string fromDoubleToScientific(double val) {
        int exp = 0;
        while (std::abs(val) < 1) {
            val *= 10;
            exp--;
        }
        while (std::abs(val) >= 10) {
            val /= 10;
            exp++;
        }
        int num = static_cast<int>(std::round(val * 10));
        std::string result = std::to_string(num);
        result.insert(1, 1, '.');
        result += 'e';
        if (exp >= 0) {
            result += '+';
        }
        return result + std::to_string(exp);
    }
}
