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

static bool parseLine(const std::string &src, DataStruct &rec) {
    if (src.size() < 4 || src.front() != '(' || src.back() != ')') return false;

    DataStruct tmp{};
    bool ok1 = false, ok2 = false, ok3 = false;
    std::string body = src.substr(1, src.size() - 2);
    size_t pos = 0;

    while (pos < body.size()) {
        size_t colon = body.find(':', pos);
        if (colon == std::string::npos) break;
        std::string field = body.substr(pos, colon - pos);
        pos = colon + 1;

        if (!ok1 && field.rfind("key1 ", 0) == 0) {
            std::string txt = field.substr(5);
            if (txt.size() > 1 && txt[0] == '0' &&
                txt.find_first_not_of("01234567") == std::string::npos) {
                tmp.key1 = std::stoull(txt, nullptr, 8);
                ok1 = true;
            }
        } else if (!ok2 && field.rfind("key2 #c(", 0) == 0) {
            size_t end = body.find(')', pos);
            if (end != std::string::npos) {
                std::istringstream iss(body.substr(pos, end - pos));
                double re, im;
                if (iss >> re >> im) {
                    tmp.key2 = {re, im};
                    ok2 = true;
                }
                pos = end + 1;
            }
        } else if (!ok3 && field.rfind("key3 \"", 0) == 0) {
            size_t end = body.find('\"', pos);
            if (end != std::string::npos) {
                tmp.key3 = body.substr(pos, end - pos);
                ok3 = true;
                pos = end + 1;
            }
        }
    }
    if (ok1 && ok2 && ok3) {
        rec = tmp;
        return true;
    }
    return false;
}

std::istream &operator>>(std::istream &in, DataStruct &record) {
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (parseLine(line, record)) return in;
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream &operator<<(std::ostream &out, const DataStruct &rec) {
    out << "(:key1 " << std::oct << rec.key1 << std::dec
            << ":key2 #c(" << rec.key2.real() << ' ' << rec.key2.imag() << ")"
            << ":key3 \"" << rec.key3 << "\":)";
    return out;
}

static bool comp(const DataStruct &a, const DataStruct &b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double na = std::abs(a.key2), nb = std::abs(b.key2);
    if (na != nb) return na < nb;
    return a.key3.size() < b.key3.size();
}

void sortData(std::vector<DataStruct> &v) {
    std::sort(v.begin(), v.end(), comp);
}

std::vector<DataStruct> parseData(std::istream &in) {
    std::vector<DataStruct> out;
    std::string line;
    while (std::getline(in, line)) {
        DataStruct rec;
        if (parseLine(trim(line), rec)) out.push_back(rec);
    }
    return out;
}

void printData(const std::vector<DataStruct> &v, std::ostream &out) {
    for (const auto &rec: v) out << rec << '\n';
}

