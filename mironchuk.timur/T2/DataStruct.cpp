#include "DataStruct.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cmath>

// Определение глобального счётчика строк
std::size_t total_lines_read = 0;

static bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1)
        return a.key1 < b.key1;
    double magA = std::abs(a.key2), magB = std::abs(b.key2);
    if (magA != magB)
        return magA < magB;
    return a.key3.length() < b.key3.length();
}

std::vector<DataStruct> parseData(std::istream& in) {
    std::vector<DataStruct> data;
    std::string line;
    total_lines_read = 0;
    while (std::getline(in, line)) {
        ++total_lines_read;
        try {
            if (line.size() < 4 || line.substr(0, 2) != "(:" || line.substr(line.size() - 2) != ":)")
                throw std::runtime_error("Invalid boundaries");

            // key1 — восьмеричный ULL
            auto p1 = line.find(":key1 ");
            if (p1 == std::string::npos) throw std::runtime_error("No key1");
            p1 += 6;
            auto e1 = line.find(':', p1);
            std::string s1 = line.substr(p1, e1 - p1);
            if (s1.size() < 2 || s1[0] != '0' || s1.find_first_not_of("01234567") != std::string::npos)
                throw std::runtime_error("Invalid key1 format");
            unsigned long long key1 = std::stoull(s1, nullptr, 8);

            // key2 — Lisp‑подобный комплекс
            auto p2 = line.find(":key2 #c(", e1);
            if (p2 == std::string::npos) throw std::runtime_error("No key2");
            p2 += 10;
            auto e2 = line.find(')', p2);
            std::string s2 = line.substr(p2, e2 - p2);
            auto sp = s2.find(' ');
            double real = std::stod(s2.substr(0, sp));
            double imag = std::stod(s2.substr(sp + 1));
            std::complex<double> key2(real, imag);

            // key3 — строка
            auto p3 = line.find(":key3 \"", e2);
            if (p3 == std::string::npos) throw std::runtime_error("No key3");
            p3 += 7;
            auto e3 = line.find('"', p3);
            std::string key3 = line.substr(p3, e3 - p3);

            data.emplace_back(DataStruct{key1, key2, key3});
        }
        catch (const std::bad_alloc&) {
            throw;
        }
        catch (...) {
            // игнорируем некорректную строку
            continue;
        }
    }
    return data;
}

void sortData(std::vector<DataStruct>& data) {
    std::sort(data.begin(), data.end(), compareData);
}

void printData(const std::vector<DataStruct>& data, std::ostream& out) {
    std::ostream_iterator<DataStruct> it(out, "\n");
    std::copy(data.begin(), data.end(), it);
}

std::ostream& operator<<(std::ostream& out, const DataStruct& ds) {
    out << "(:key1 " << std::oct << ds.key1 << std::dec;
    out << ":key2 #c(" << ds.key2.real() << " " << ds.key2.imag() << ")";
    out << ":key3 \"" << ds.key3 << "\":)";
    return out;
}
