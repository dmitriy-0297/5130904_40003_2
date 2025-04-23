#include "record_sort.hpp"
#include <cmath>

bool compareRecords(const DataStruct &a, const DataStruct &b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    double ma = std::abs(a.key2), mb = std::abs(b.key2);
    if (ma != mb) return ma < mb;
    return a.key3.size() < b.key3.size();
}
