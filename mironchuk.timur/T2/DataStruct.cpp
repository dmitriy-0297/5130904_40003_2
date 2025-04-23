#include "DataStruct.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <cmath>

static std::string trim(const std::string &s) {
    auto b = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto e = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    return (b < e) ? std::string(b, e) : std::string();
}

static bool parseLine(const std::string &line, DataStruct &out) {
    if (line.size() < 4 || line.front() != '(' || line.back() != ')') return false;

    DataStruct ds{};
    bool k1 = false, k2 = false, k3 = false;

    std::string content = line.substr(1, line.size() - 2);
    size_t pos = 0;
    while (pos < content.size()) {
        size_t colon = content.find(':', pos);
        if (colon == std::string::npos) break;
        std::string field = content.substr(pos, colon - pos);
        pos = colon + 1;

        if (!k1 && field.rfind("key1 ", 0) == 0) {
            std::string v = field.substr(5);
            if (v.size() > 1 && v[0] == '0' && v.find_first_not_of("01234567") == std::string::npos) {
                ds.key1 = std::stoull(v, nullptr, 8);
                k1 = true;
            }
        } else if (!k2 && field.rfind("key2 #c(", 0) == 0) {
            size_t end = content.find(')', pos);
            if (end != std::string::npos) {
                std::istringstream iss(content.substr(pos, end - pos));
                double re, im;
                if (iss >> re >> im) {
                    ds.key2 = {re, im};
                    k2 = true;
                }
                pos = end + 1;
            }
        } else if (!k3 && field.rfind("key3 \"", 0) == 0) {
            size_t end = content.find('"', pos);
            if (end != std::string::npos) {
                ds.key3 = content.substr(pos, end - pos);
                k3 = true;
                pos = end + 1;
            }
        }
    }
    if (k1 && k2 && k3) {
        out = ds;
        return true;
    }
    return false;
}

std::istream &operator>>(std::istream &in, DataStruct &data) {
    std::string raw;
    while (std::getline(in, raw)) {
        raw = trim(raw);
        if (raw.empty()) continue;
        if (parseLine(raw, data)) return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream &operator<<(std::ostream &out, const DataStruct &data) {
    out << "(:key1 " << std::oct << data.key1 << std::dec
            << ":key2 #c(" << data.key2.real() << ' ' << data.key2.imag() << ")"
            << ":key3 \"" << data.key3 << "\":)";
    return out;
}

static bool compare(const DataStruct &a, const DataStruct &b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double ma = std::abs(a.key2), mb = std::abs(b.key2);
    if (ma != mb) return ma < mb;
    return a.key3.size() < b.key3.size();
}

void sortData(std::vector<DataStruct> &data) {
    std::sort(data.begin(), data.end(), compare);
}

std::vector<DataStruct> parseData(std::istream &in) {
    std::vector<DataStruct> v;
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        DataStruct ds;
        if (parseLine(line, ds)) v.push_back(ds);
    }
    return v;
}

void printData(const std::vector<DataStruct> &data, std::ostream &out) {
    for (const auto &d: data) out << d << '\n';
}

