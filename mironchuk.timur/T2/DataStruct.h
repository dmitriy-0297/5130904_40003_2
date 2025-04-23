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

std::istream &operator>>(std::istream &in, DataStruct &data);

std::ostream &operator<<(std::ostream &out, const DataStruct &data);

void printData(const std::vector<DataStruct> &data, std::ostream &out);

std::vector<DataStruct> parseData(std::istream &in);

void sortData(std::vector<DataStruct> &data);

#endif // DATASTRUCT_H

