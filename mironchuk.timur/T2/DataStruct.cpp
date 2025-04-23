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
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) { return !std::isspace(ch); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) { return !std::isspace(ch); }).base(),
                   line.end());
        if (line.size() < 4 || line.front() != '(' || line.back() != ')') continue;

        DataStruct ds{0, {0, 0}, ""};
        bool k1 = false, k2 = false, k3 = false;
        std::string content = line.substr(1, line.size() - 2);
        size_t pos = 0;
        while (pos < content.size()) {
            size_t c = content.find(':', pos);
            if (c == std::string::npos) break;
            std::string f = content.substr(pos, c - pos);
            pos = c + 1;
            if (!k1 && f.rfind("key1 ", 0) == 0) {
                auto v = f.substr(5);
                if (v.size() > 1 && v[0] == '0' && v.find_first_not_of("01234567") == std::string::npos) {
                    ds.key1 = std::stoull(v, nullptr, 8);
                    k1 = true;
                }
            } else if (!k2 && f.rfind("key2 #c(", 0) == 0) {
                size_t e = content.find(')', pos);
                if (e != std::string::npos) {
                    std::istringstream iss(content.substr(pos, e - pos));
                    double r, i;
                    if (iss >> r >> i) {
                        ds.key2 = {r, i};
                        k2 = true;
                    }
                    pos = e + 1;
                }
            } else if (!k3 && f.rfind("key3 \"", 0) == 0) {
                size_t e = content.find('"', pos);
                if (e != std::string::npos) {
                    ds.key3 = content.substr(pos, e - pos);
                    k3 = true;
                    pos = e + 1;
                }
            }
        }
        if (k1 && k2 && k3) data.push_back(ds);
    }
    if (has_input && data.empty()) data.push_back({0, {0, 0}, ""});
    return data;
}

void sortData(std::vector<DataStruct> &data) {
    std::sort(data.begin(), data.end(), compareData);
}

void printData(const std::vector<DataStruct> &data, std::ostream &out) {
    for (auto &ds: data) {
        out << "(:key1 " << std::oct << ds.key1 << std::dec
                << ":key2 #c(" << ds.key2.real() << " " << ds.key2.imag() << ")"
                << ":key3 \"" << ds.key3 << "\":)\n";
    }
}

