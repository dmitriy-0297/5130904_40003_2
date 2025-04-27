#include "record_sort.hpp"
#include <cmath>

bool compare(const DataStruct &ds1, const DataStruct &ds2) {
    if (ds1.key1 != ds2.key1) {
        return ds1.key1 < ds2.key1;
    }
    if (ds1.key2 != ds2.key2) {
        return std::abs(ds1.key2) < std::abs(ds2.key2);
    }
    return ds1.key3.size() < ds2.key3.size();
}

