#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <vector>
#include <complex>
#include <istream>
#include <ostream>

struct DataStruct {
    unsigned long long       key1;
    std::complex<double>     key2;
    std::string              key3;
};

// Читает все записи из входного потока, игнорируя некорректные
std::vector<DataStruct> parseData(std::istream& in);

// Сортирует: по key1, затем по |key2|, затем по длине key3
void sortData(std::vector<DataStruct>& data);

// Печатает все записи в выходной поток
void printData(const std::vector<DataStruct>& data, std::ostream& out);

// Перегрузка оператора для вывода одной структуры
std::ostream& operator<<(std::ostream& out, const DataStruct& ds);

#endif // DATASTRUCT_H
