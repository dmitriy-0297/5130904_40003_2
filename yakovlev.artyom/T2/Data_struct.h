#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iosfwd>
#include <string>
#include <utility>

namespace yakovlev
{
    struct DataStruct
    {
        double                                     key1{};
        std::pair<long long, unsigned long long>   key2{};
        std::string                                key3{};
    };

    struct Del { char  ch; };
    struct Lbl { const char* literal; };
    struct Dbl { double& v; };
    struct Rat {
        std::pair<long long,
            unsigned long long>& v;
    };
    struct Str { std::string& v; };

    std::istream& operator>>(std::istream&, DataStruct&) noexcept;
    std::ostream& operator<<(std::ostream&, const DataStruct&) noexcept;
    bool            operator<(const DataStruct&, const DataStruct&) noexcept;

}
#endif
