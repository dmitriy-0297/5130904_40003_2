#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <vector>
#include <complex>
#include <istream>
#include <ostream>
#include <cstddef>

struct DataStruct {
    unsigned long long       key1;
    std::complex<double>     key2;
    std::string              key3;
};

extern std::size_t total_lines_read;

std::vector<DataStruct> parseData(std::istream& in);

void sortData(std::vector<DataStruct>& data);

void printData(const std::vector<DataStruct>& data, std::ostream& out);

std::ostream& operator<<(std::ostream& out, const DataStruct& ds);

#endif // DATASTRUCT_H


