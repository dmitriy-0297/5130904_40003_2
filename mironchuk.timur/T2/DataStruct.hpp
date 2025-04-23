#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <string>
#include <complex>
#include <istream>
#include <ostream>

struct DataStruct {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

std::istream &operator>>(std::istream &in, DataStruct &value);

std::ostream &operator<<(std::ostream &out, const DataStruct &value);

#endif // DATA_STRUCT_HPP
