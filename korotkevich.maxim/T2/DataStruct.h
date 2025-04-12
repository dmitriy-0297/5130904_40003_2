#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <complex>
#include <string>
#include <regex>

namespace krrmaxim
{
const std::regex REGEXPATTERNKEY1("^0[xX][0-9A-Fa-f]+$");
const std::regex REGEXPATTERNKEY2("^#c\\([-]?\\d+(\\.\\d+)?\\s[-]?\\d+(\\.\\d+)?\\)$");
const std::regex REGEXPATTERNKEY3("^\"[^\"]*\"$");


struct DataStruct
{
  unsigned long long key1;
  std::complex<double> key2;
  std::string key3;
};

std::istream& operator>>(std::istream& input, DataStruct& data);
std::ostream& operator<<(std::ostream& output, const DataStruct& data);
bool dataStructComparator(const DataStruct& d1, const DataStruct& d2);
}

#endif