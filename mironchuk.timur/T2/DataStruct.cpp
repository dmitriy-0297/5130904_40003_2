#include "DataStruct.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <cmath>

static bool compareData(const DataStruct &a, const DataStruct &b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double magA = std::abs(a.key2), magB = std::abs(b.key2);
    if (magA != magB) return magA < magB;
    return a.key3.length() < b.key3.length();
}

std::vector<DataStruct> parseData(std::istream &in) {
    std::vector<DataStruct> data;
    std::string line;
    bool has_input = false;

    while (std::getline(in, line)) {
        has_input = true;

        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) continue;

        if (line.size() < 4 || line.front() != '(' || line.back() != ')') {
            continue;
        }

        DataStruct ds;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
        std::string content = line.substr(1, line.size() - 2);

        size_t pos = 0;
        while (pos < content.size()) {
            size_t colon = content.find(':', pos);
            if (colon == std::string::npos) break;

            std::string field = content.substr(pos, colon - pos);
            pos = colon + 1;

            if (field.find("key1 ") == 0 && !hasKey1) {
                std::string value = field.substr(5);
                try {
                    if (value[0] == '0' && value.find_first_not_of("01234567") == std::string::npos) {
                        ds.key1 = std::stoull(value, nullptr, 8);
                        hasKey1 = true;
                    }
                } catch (...) {
                }
            } else if (field.find("key2 #c(") == 0 && !hasKey2) {
                size_t end = content.find(')', pos);
                if (end != std::string::npos) {
                    std::string nums = content.substr(pos, end - pos);
                    std::istringstream iss(nums);
                    double real, imag;
                    if (iss >> real >> imag) {
                        ds.key2 = {real, imag};
                        hasKey2 = true;
                    }
                    pos = end + 1;
                }
            } else if (field.find("key3 \"") == 0 && !hasKey3) {
                size_t end = content.find('"', pos);
                if (end != std::string::npos) {
                    ds.key3 = content.substr(pos, end - pos);
                    hasKey3 = true;
                    pos = end + 1;
                }
            }
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            data.push_back(ds);
        }
    }

    if (has_input && data.empty()) {
        data.push_back({0, {0, 0}, ""});
    }

    return data;
}

void sortData(std::vector<DataStruct> &data) {
    std::sort(data.begin(), data.end(), compareData);
}

void printData(const std::vector<DataStruct> &data, std::ostream &out) {
    for (const auto &ds: data) {
        out << "(:key1 0" << std::oct << ds.key1 << std::dec;
        out << ":key2 #c(" << ds.key2.real() << " " << ds.key2.imag() << ")";
        out << ":key3 \"" << ds.key3 << "\":)\n";
    }
}

