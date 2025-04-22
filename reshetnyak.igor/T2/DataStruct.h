#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>
#include <regex>
#include <complex>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cmath>
#include <iomanip>

const std::regex REGEX_SLL_LIT(R"(^[-]?([1-9][0-9]*)(ll|LL)$)");
const std::regex REGEX_CMP_LSP(R"(^#c\(([-]?\d+\.\d+)\s([-]?\d+\.\d+)\)$)");

const long long INVALID_LL_INDICATOR = 0ll;
const std::complex<double> INVALID_CMP_INDICATOR = std::complex<double>(0, 0);
const std::string INVALID_STR_INDICATOR = "";

struct DataStruct {
    long long key1;
    std::complex<double> key2;
    std::string key3;
};

class StreamGuard {
public:
    StreamGuard(std::basic_ios<char>& s);
    ~StreamGuard();

private:
    std::basic_ios<char>& s_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags flags_;
};

std::ostream& operator<<(std::ostream& out, const DataStruct& data);
std::istream& operator>>(std::istream& in, DataStruct& data);
bool compareDataStruct(const DataStruct& a, const DataStruct& b);
DataStruct getValues(const std::string& input);

#endif // DATA_STRUCT_H
