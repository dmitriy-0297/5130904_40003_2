#ifndef DataStruct_hpp
#define DataStruct_hpp

#include <string>
#include <iostream>
#include <complex>

struct DataStruct {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

std::ostream &operator<<(std::ostream &out, const DataStruct &dest);

std::istream &operator>>(std::istream &in, DataStruct &dest);

#endif DataStruct_hpp

