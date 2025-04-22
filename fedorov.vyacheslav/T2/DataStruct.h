#include <string>
#include <iostream>
#include <utility>

struct DataStruct {
  long long key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};
std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
bool cusComparator(const DataStruct& a, const DataStruct& b);
bool parseRecord(const std::string& line, DataStruct& data);
