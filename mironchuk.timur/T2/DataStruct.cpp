#include "DataStruct.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <cctype>

std::size_t total_lines_read = 0;

static bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double magA = std::abs(a.key2), magB = std::abs(b.key2);
    if (magA != magB) return magA < magB;
    return a.key3.length() < b.key3.length();
}

std::vector<DataStruct> parseData(std::istream& in) {
    std::vector<DataStruct> data;
    std::string line;
    total_lines_read = 0;

    while (std::getline(in, line)) {
        ++total_lines_read;

        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) { return !std::isspace(ch); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) { return !std::isspace(ch); }).base(), line.end());

        if (line.size() < 4 || line.front() != '(' || line.back() != ')') {
            continue;
        }

        std::string content = line.substr(1, line.size() - 2);

        DataStruct ds;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        size_t start = 0;
        while (start < content.size()) {
            size_t end = content.find(':', start);
            if (end == std::string::npos) end = content.size();
            
            std::string field = content.substr(start, end - start);
            if (!field.empty()) {
                if (!hasKey1 && field.find("key1 ") == 0) {
                    std::string value = field.substr(5);
                    try {
                        if (value[0] == '0' && value.find_first_not_of("01234567") == std::string::npos) {
                            ds.key1 = std::stoull(value, nullptr, 8);
                            hasKey1 = true;
                        }
                    } catch (...) {}
                }
                else if (!hasKey2 && field.find("key2 #c(") == 0) {
                    size_t end_paren = field.rfind(')');
                    if (end_paren != std::string::npos) {
                        std::string nums = field.substr(7, end_paren - 7);
                        std::istringstream iss(nums);
                        double real, imag;
                        if (iss >> real >> imag) {
                            ds.key2 = std::complex<double>(real, imag);
                            hasKey2 = true;
                        }
                    }
                }
                else if (!hasKey3 && field.find("key3 \"") == 0) {
                    size_t end_quote = field.rfind('"');
                    if (end_quote != std::string::npos && end_quote > 6) {
                        ds.key3 = field.substr(6, end_quote - 6);
                        hasKey3 = true;
                    }
                }
            }
            
            start = end + 1;
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            data.push_back(ds);
        }
    }
    return data;
}

void sortData(std::vector<DataStruct>& data) {
    std::sort(data.begin(), data.end(), compareData);
}

void printData(const std::vector<DataStruct>& data, std::ostream& out) {
    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(out, "\n"));
}

std::ostream& operator<<(std::ostream& out, const DataStruct& ds) {
    out << "(:key1 0" << std::oct << ds.key1 << std::dec;
    out << ":key2 #c(" << ds.key2.real() << " " << ds.key2.imag() << ")";
    out << ":key3 \"" << ds.key3 << "\":)";
    return out;
}
