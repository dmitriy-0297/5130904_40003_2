#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>

struct DataStruct
{
  char key1 = ' ';
  std::pair<long long, unsigned long long> key2 = { 0, 0 };
  std::string key3 = "";
};

std::ostream& operator<< (std::ostream& out, const DataStruct& data);
std::istream& operator>> (std::istream& in, DataStruct& data);

#endif

