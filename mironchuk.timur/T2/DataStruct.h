#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <sstream>
#include <iterator>
#include <algorithm>

struct DataStruct {
    unsigned long long key1; // ULL OCT
    std::complex<double> key2; // CMP LSP
    std::string key3;
};

struct Delimiter {
    char value;
};

struct UllOct {
    unsigned long long &value;
};

struct ComplexNum {
    std::complex<double> &value;
};

struct String {
    std::string &value;
};

bool compareDataStructs(const DataStruct &a, const DataStruct &b);

std::istream &operator>>(std::istream &in, Delimiter &&data);

std::istream &operator>>(std::istream &in, UllOct &&data);

std::istream &operator>>(std::istream &in, ComplexNum &&data);

std::istream &operator>>(std::istream &in, String &&data);

std::istream &operator>>(std::istream &in, DataStruct &data);

std::ostream &operator<<(std::ostream &out, const DataStruct &data);

#endif

