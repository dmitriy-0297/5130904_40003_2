#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <vector>
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

std::vector<DataStruct> parseData(std::istream &in);

void sortData(std::vector<DataStruct> &v);

void printData(const std::vector<DataStruct> &v,
               std::ostream &out);

#endif // DATASTRUCT_H

