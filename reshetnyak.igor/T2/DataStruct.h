#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include <regex>
#include <complex>
#include <cmath>

const std::regex REGEX_SLL_LIT(R"(^[-]?([0]|[1-9][0-9]*)(ll|LL)$)");
const std::regex REGEX_CMP_LSP(R"(^#c\(([-]?\d+\.\d+)\s([-]?\d+\.\d+)\)$)");

struct DataStruct
{
    long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO
{
    char exp;
};

struct LabelIO
{
    std::string exp;
};

class StreamGuard
{
public:
    StreamGuard(std::basic_ios<char>& s);
    ~StreamGuard();
private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& data);
std::istream& operator>>(std::istream& in, LabelIO& data);
std::istream& operator>>(std::istream& in, DataStruct& input);
std::ostream& operator<<(std::ostream& out, const DataStruct& output);
std::complex<double> parse_complex(const std::string& input);
bool compare(const DataStruct& a, const DataStruct& b);
#endif
