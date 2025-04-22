#include "DataStruct.h"
#include <regex>

bool parseRecord(const std::string& line, DataStruct& data) {
  std::regex key1_re(":key1\\s+([-+]?[0-9]+)(?:ll|LL|ull)?:");
  std::regex key2_re(":key2\\s+\\(\\s*:N\\s+([-+]?[0-9]+):D\\s+([0-9]+):\\s*\\):");
  std::regex key3_re(":key3\\s+\"([^\"]*)\":");

  std::smatch match;

  if (!std::regex_search(line, match, key1_re)) {
    return false;
  }
  try {
    data.key1 = std::stoll(match[1]);
  }
  catch (const std::invalid_argument& e) {
    return false;
  }
  catch (const std::out_of_range& e) {
    return false;
  }

  if (!std::regex_search(line, match, key2_re)) {
    return false;
  }
  try {
    data.key2.first = std::stoll(match[1]);
    data.key2.second = std::stoull(match[2]);
  }
  catch (const std::invalid_argument& e) {
    return false;
  }
  catch (const std::out_of_range& e) {
    return false;
  }

  if (!std::regex_search(line, match, key3_re)) {
    return false;
  }
  data.key3 = match[1];

  return true;
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
  std::string line;
  if (!std::getline(in, line)) {
    return in;
  }
  if (line.empty()) {
    in.setstate(std::ios::eofbit);
    return in;
  }
  if (line.front() != '(' || line.back() != ')') {
    in.setstate(std::ios::failbit);
    return in;
  }
  if (!parseRecord(line, data)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
  out << "(:key1 " << data.key1 << "ll:"
    << "key2 (:N " << data.key2.first << ":D " << data.key2.second << ":):"
    << "key3 \"" << data.key3 << "\":)";
  return out;
}


bool cusComparator(const DataStruct& a, const DataStruct& b) {
  if (a.key1 != b.key1) {
    return a.key1 < b.key1;
  }
  if (a.key2.first != b.key2.first) {
    return a.key2.first < b.key2.first;
  }
  if (a.key2.second != b.key2.second) {
    return a.key2.second < b.key2.second;
  }
  return a.key3.size() < b.key3.size();
}
