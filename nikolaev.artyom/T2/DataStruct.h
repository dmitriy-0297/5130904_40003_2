#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <regex>

namespace artttnik
{
const std::regex PATTERNFORKEY1(R"(^[-+]?[1-9](\.\d+)?[eE][-+]?\d+$)");
const std::regex PATTERNFORKEY2("^-?[0-9]+(ll|LL)$");
const std::regex PATTERNFORKEY3("\".*\"");

struct DataStruct
{
  double key1;    // DBL SCI 5.45e-2
  long long int key2; // SLL LIT -89LL
  std::string key3;
};

std::istream &operator>>(std::istream &input, DataStruct &data);
std::ostream &operator<<(std::ostream &output, const DataStruct &data);
bool compareDataStruct(const DataStruct &data1, const DataStruct &data2);

bool isDouble(const std::string& str);
bool isLLong(const std::string& str);
bool isString(const std::string& str);
std::string makeScientific(const double &data);
}

#endif
