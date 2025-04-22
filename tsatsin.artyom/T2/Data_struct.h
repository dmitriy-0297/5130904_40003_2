#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <iostream>

namespace artemonts
{
    struct DataStruct
    {
        double key1;
        unsigned long long key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in, DataStruct& value);
    std::ostream& operator<<(std::ostream& out, const DataStruct& value);
    bool operator<(const DataStruct& value1, const DataStruct& value2);

    struct StringIO
    {
        std::string& val;
    };

    struct DoubleIO
    {
        double& val;
    };

    struct UnsignedllIO
    {
        unsigned long long& val;
    };

    struct DelimiterIO
    {
        char val;
    };

    struct LabelIO
    {
        std::string val;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& val);
    std::istream& operator>>(std::istream& in, StringIO&& val);
    std::istream& operator>>(std::istream& in, DoubleIO&& val);
    std::istream& operator>>(std::istream& in, UnsignedllIO&& val);
    std::istream& operator>>(std::istream& in, LabelIO&& val);

    std::string fromDoubleToScientific(double val);
    std::string fromULLtoBinary(unsigned long long value);
}

#endif
