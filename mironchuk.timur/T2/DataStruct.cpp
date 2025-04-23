#include "DataStruct.h"
#include <cctype>
#include <limits>

bool compareDataStructs(const DataStruct &a, const DataStruct &b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double magA = std::abs(a.key2), magB = std::abs(b.key2);
    if (magA != magB) return magA < magB;
    return a.key3.size() < b.key3.size();
}

std::istream &operator>>(std::istream &in, Delimiter &&data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    char c = '0';
    in >> c;
    if (in && (std::tolower(c) != std::tolower(data.value))) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream &operator>>(std::istream &in, UllOct &&data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    std::string numStr;
    in >> numStr;

    try {
        if (numStr.empty() || numStr[0] != '0' ||
            numStr.find_first_not_of("01234567") != std::string::npos) {
            in.setstate(std::ios::failbit);
            return in;
        }
        data.value = std::stoull(numStr, nullptr, 8);
    } catch (...) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream &operator>>(std::istream &in, ComplexNum &&data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    in >> Delimiter{'#'} >> Delimiter{'c'} >> Delimiter{'('};
    double real, imag;
    in >> real >> imag >> Delimiter{')'} >> Delimiter{':'};

    if (in) {
        data.value = std::complex<double>(real, imag);
    }
    return in;
}

std::istream &operator>>(std::istream &in, String &&data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    std::getline(in >> Delimiter{'"'}, data.value, '"');
    if (in.fail()) in.setstate(std::ios::failbit);
    in >> Delimiter{':'};
    return in;
}

std::istream &operator>>(std::istream &in, DataStruct &data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;

    DataStruct temp;
    in >> Delimiter{'('} >> Delimiter{':'};

    for (size_t i = 0; i < 3; ++i) {
        std::string key;
        in >> key;
        if (key == "key1") {
            in >> UllOct{temp.key1};
        } else if (key == "key2") {
            in >> ComplexNum{temp.key2};
        } else if (key == "key3") {
            in >> String{temp.key3};
        } else {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    in >> Delimiter{')'};
    if (in) data = temp;
    return in;
}

std::ostream &operator<<(std::ostream &out, const DataStruct &data) {
    out << "(:key1 0" << std::oct << data.key1 << std::dec
            << ":key2 #c(" << data.key2.real() << " " << data.key2.imag() << ")"
            << ":key3 \"" << data.key3 << "\":)";
    return out;
}

