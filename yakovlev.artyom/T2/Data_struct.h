#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iosfwd>
#include <string>
#include <utility>
#include <complex>

namespace yakovlevart
{
    struct DataStruct
    {
        double key1{};
        std::pair<long long, unsigned long long> key2{};
        std::string key3{};
    };

    std::istream& operator>>(std::istream& in, DataStruct& value);
    std::ostream& operator<<(std::ostream& out, const DataStruct& value);
    bool operator<(const DataStruct& lhs, const DataStruct& rhs);

    struct DoubleIO { double& val; };
    struct RationalIO { std::pair<long long, unsigned long long>& val; };
    struct StringIO { std::string& val; };
    struct DelimiterIO { char val; };
    struct LabelIO { std::string val; };

    std::istream& operator>>(std::istream& in, DelimiterIO&& val);
    std::istream& operator>>(std::istream& in, LabelIO&& val);
    std::istream& operator>>(std::istream& in, StringIO&& val);
    std::istream& operator>>(std::istream& in, DoubleIO&& val);
    std::istream& operator>>(std::istream& in, RationalIO&& val);
}

#endif
