#include "DataStruct.hpp"
#include "fmt_guard.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <cmath>

namespace {
    std::string trim(const std::string &str) {
        auto b = std::find_if_not(str.begin(), str.end(), ::isspace);
        auto e = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
        return (b < e) ? std::string(b, e) : std::string();
    }

    bool parseLine(const std::string &line, DataStruct &out) {
        if (line.size() < 4 || line.front() != '(' || line.back() != ')') return false;

        DataStruct tmp{};
        bool k1 = false, k2 = false, k3 = false;
        std::string body = line.substr(1, line.size() - 2);
        size_t pos = 0;
        while (pos < body.size()) {
            size_t col = body.find(':', pos);
            if (col == std::string::npos) break;
            std::string field = body.substr(pos, col - pos);
            pos = col + 1;
            if (!k1 && field.rfind("key1 ", 0) == 0) {
                std::string val = field.substr(5);
                if (val.size() > 1 && val[0] == '0' && val.find_first_not_of("01234567") == std::string::npos) {
                    tmp.key1 = std::stoull(val, nullptr, 8);
                    k1 = true;
                }
            } else if (!k2 && field.rfind("key2 #c(", 0) == 0) {
                size_t end = body.find(')', pos);
                if (end != std::string::npos) {
                    std::istringstream iss(body.substr(pos, end - pos));
                    double re, im;
                    if (iss >> re >> im) {
                        tmp.key2 = {re, im};
                        k2 = true;
                    }
                    pos = end + 1;
                }
            } else if (!k3 && field.rfind("key3 \"", 0) == 0) {
                size_t end = body.find('"', pos);
                if (end != std::string::npos) {
                    tmp.key3 = body.substr(pos, end - pos);
                    k3 = true;
                    pos = end + 1;
                }
            }
        }
        if (k1 && k2 && k3) {
            out = tmp;
            return true;
        }
        return false;
    }

    std::istream &operator>>(std::istream &in, DataStruct &value) {
        std::string line;
        while (std::getline(in, line)) {
            line = trim(line);
            if (line.empty()) continue;
            if (parseLine(line, value)) return in;
        }
        in.setstate(std::ios::failbit);
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const DataStruct &v) {
        IOFmtGuard g(out);
        out << "(:key1 0" << std::oct << v.key1 << std::dec;
        out << ":key2 #c(" << std::fixed << std::setprecision(1)
                << v.key2.real() << ' ' << v.key2.imag() << ')';
        out << ":key3 \"" << v.key3 << "\":)";
        return out;
    }

