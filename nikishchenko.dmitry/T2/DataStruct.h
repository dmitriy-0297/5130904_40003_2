#ifndef DATA_STRUCT
#define DATA_STRUCT

#include <string>
#include <iostream>
#include <cctype>
#include <regex>
#include <iomanip>

const int LABEL_LENGTH = 4;
const int OPTION_ONE = 49;
const int OPTION_TWO = 50;
const int OPTION_THREE = 51;

const double INVALID_DOUBLE_INDICATOR = 0.0;
const long long INVALID_LL_INDICATOR = 0ll;
const std::string INVALID_STR_INDICATOR = "";

const std::string REGEX_DBL_LIT_DOUBLE = "^([0]|[1-9][0-9]*)(\.([0-9][0-9]?))[dD]?$";
const std::string REGEX_DBL_LIT_INT = "^([0]|[1-9][0-9]*)[dD]?$";
const std::string REGEX_SLL = "^[+-]?([0]|[1-9][0-9]*)(ll||LL)?$";

struct DataStruct
{
    double key1;
    long long key2;
    std::string key3;
};

struct SeparatorIO
{
    const char exp;
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
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

std::istream& operator>>(std::istream& in, DataStruct& input);
std::ostream& operator<<(std::ostream& out, const DataStruct& output);
std::istream& operator>>(std::istream& in, SeparatorIO&& sep);
std::istream& operator>>(std::istream& in, LabelIO& sep);
void ignoreInput(std::istream& in);
std::string getCorrectRound(const double& num);
bool compareDataStruct(const DataStruct& first, const DataStruct& second);

#endif
