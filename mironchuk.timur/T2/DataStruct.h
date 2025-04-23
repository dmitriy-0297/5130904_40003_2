#include "DataStruct.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sstream>
#include <cctype>

std::size_t total_lines_read = 0;
std::size_t total_lines_parsed = 0;

static bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double magA = std::abs(a.key2), magB = std::abs(b.key2);
    if (magA != magB) return magA < magB;
    return a.key3.length() < b.key3.length();
}

static bool parseKey1(const std::string& field, unsigned long long& key1) {
    const std::string prefix = "key1 ";
    if (field.find(prefix) != 0) return false;

    std::string value = field.substr(prefix.size());
    if (value.empty()) return false;

    try {
        if (value[0] != '0' || value.find_first_not_of("01234567") != std::string::npos) {
            return false;
        }
        key1 = std::stoull(value, nullptr, 8);
        return true;
    } catch (...) {
        return false;
    }
}

static bool parseKey2(const std::string& field, std::complex<double>& key2) {
    const std::string prefix = "key2 #c(";
    const std::string suffix = ")";

    size_t start = field.find(prefix);
    if (start != 0) return false;

    size_t end = field.rfind(suffix);
    if (end == std::string::npos || end <= start + prefix.size()) return false;

    std::string content = field.substr(prefix.size(), end - prefix.size());
    std::istringstream iss(content);
    double real, imag;

    if (!(iss >> real >> imag)) return false;
    key2 = std::complex<double>(real, imag);
    return true;
}

static bool parseKey3(const std::string& field, std::string& key3) {
    const std::string prefix = "key3 \"";
    const std::string suffix = "\"";

    size_t start = field.find(prefix);
    if (start != 0) return false;

    size_t end = field.rfind(suffix);
    if (end == std::string::npos || end <= start + prefix.size()) return false;

    key3 = field.substr(prefix.size(), end - prefix.size());
    return true;
}

std::vector<DataStruct> parseData(std::istream& in) {
    std::vector<DataStruct> data;
    std::string line;
    total_lines_read = 0;
    total_lines_parsed = 0;

    while (std::getline(in, line)) {
        ++total_lines_read;

        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) { return !std::isspace(ch); }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) { return !std::isspace(ch); }).base(), line.end());

        // Check boundaries
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
                if (!hasKey1 && parseKey1(field, ds.key1)) hasKey1 = true;
                else if (!hasKey2 && parseKey2(field, ds.key2)) hasKey2 = true;
                else if (!hasKey3 && parseKey3(field, ds.key3)) hasKey3 = true;
            }

            start = end + 1;
        }

        if (hasKey1 && hasKey2 && hasKey3) {
            data.push_back(ds);
            ++total_lines_parsed;
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
