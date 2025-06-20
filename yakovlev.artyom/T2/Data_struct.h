#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iosfwd>
#include <string>
#include <utility>

namespace yakovlevart
{
    struct DataStruct
    {
        double key1{};
        std::pair<long long, unsigned long long>  key2{};
        std::string                               key3{};
    };

    std::istream& operator>>(std::istream& in, DataStruct& value) noexcept;
    std::ostream& operator<<(std::ostream& out, const DataStruct& value) noexcept;
    bool operator<(const DataStruct& lhs, const DataStruct& rhs) noexcept;

    struct DoubleIO
    {
        double& val;
    };
    struct RationalIO
    {
        std::pair<long long, unsigned long long>& val;
    };
    struct StringIO
    {
        std::string& val;
    };
    struct DelimiterIO
    {
        char val;
    };
    struct LabelIO
    {
        std::string val;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& val) noexcept;
    std::istream& operator>>(std::istream& in, LabelIO&& val) noexcept;
    std::istream& operator>>(std::istream& in, StringIO&& val) noexcept;
    std::istream& operator>>(std::istream& in, DoubleIO&& val) noexcept;
    std::istream& operator>>(std::istream& in, RationalIO&& val) noexcept;
}

#endif
