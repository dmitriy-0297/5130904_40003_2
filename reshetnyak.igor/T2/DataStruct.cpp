#include "DataStruct.h"

std::complex<double> parseComplexManual(const std::string& str) {
    size_t start = str.find('(') + 1;
    size_t end = str.find(')');
    std::string content = str.substr(start, end - start);

    std::istringstream iss(content);
    double real, imag;
    iss >> real >> imag;

    return std::complex<double>(real, imag);
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    StreamGuard guard(out);
    out << "(:key1 " << data.key1 << "ll";
    out << ":key2 #c(" << std::fixed << std::setprecision(1)
        << data.key2.real() << " " << data.key2.imag() << ")";
    out << ":key3 \"" << data.key3 << "\":)";
    return out;
}

DataStruct getValues(const std::string& input)
{
    DataStruct temp = {
        INVALID_LL_INDICATOR,
        INVALID_CMP_INDICATOR,
        INVALID_STR_INDICATOR
    };

    std::string str = input;
    if (str.front() != '(' || str.back() != ')') return temp;
    str = str.substr(1, str.size() - 2);

    auto find_between = [](const std::string& source, const std::string& startKey, const std::string& endKey = "") {
        size_t start = source.find(startKey);
        if (start == std::string::npos) return std::string();
        start += startKey.length();

        size_t end = endKey.empty() ? std::string::npos : source.find(endKey, start);
        return source.substr(start, end - start);
        };

    std::string val1 = find_between(str, ":key1 ", ":key2");
    std::string val2 = find_between(str, ":key2 ", ":key3");
    std::string val3 = find_between(str, ":key3 \"");
    size_t quote_end = val3.find('"');
    val3 = quote_end != std::string::npos ? val3.substr(0, quote_end) : "";

    // key1
    if (std::regex_match(val1, REGEX_SLL_LIT))
    {
        std::string num = val1.substr(0, val1.find("ll"));
        temp.key1 = std::stoll(num);
    }
    else
    {
        return temp;
    }

    // key2
    std::smatch match;
    if (std::regex_match(val2, match, REGEX_CMP_LSP))
    {
        temp.key2 = std::complex<double>(std::stod(match[1]), std::stod(match[2]));
    }
    else
    {
        return temp;
    }

    // key3
    if (!val3.empty())
    {
        temp.key3 = val3;
    }
    else
    {
        return temp;
    }

    return temp;
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);

    if (!sentry)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    std::string line;
    char ch;

    while (in >> ch && ch != '(');
    if (in.eof())
    {
        in.setstate(std::ios::eofbit);
        return in;
    }

    line += ch;

    int depth = 1;
    while (in.get(ch)) {
        line += ch;
        if (ch == '(') ++depth;
        else if (ch == ')') {
            --depth;
            if (depth == 0) break;
        }
    }

    if (depth != 0) {
        return in;
    }

    DataStruct parsed = getValues(line);

    if (parsed.key1 == INVALID_LL_INDICATOR || parsed.key2 == INVALID_CMP_INDICATOR || parsed.key3 == INVALID_STR_INDICATOR)
    {
        return in;
    }

    dest = std::move(parsed);
    return in;
}

bool compareDataStruct(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }

    double absA = std::abs(a.key2);
    double absB = std::abs(b.key2);
    if (absA != absB)
    {
        return absA < absB;
    }

    return a.key3.length() < b.key3.length();
}

StreamGuard::StreamGuard(std::basic_ios<char>& s) : s_(s)
{
    precision_ = s.precision();
    flags_ = s.flags();
}

StreamGuard::~StreamGuard()
{
    s_.precision(precision_);
    s_.flags(flags_);
}
