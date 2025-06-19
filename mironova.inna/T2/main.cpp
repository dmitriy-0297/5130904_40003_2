#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <iterator>
#include <sstream>

using namespace std;

struct DataStruct
{
private:
    unsigned long long key1_;
    unsigned long long key2_;
    string key3_;

    bool compare(const DataStruct& other) const
    {
        if (key1_ > other.key1_) return true;
        if (key1_ < other.key1_) return false;

        if (key2_ > other.key2_) return true;
        if (key2_ < other.key2_) return false;

        if (key3_ > other.key3_) return true;
        if (key3_ < other.key3_) return false;

        return false;
    }

    bool isNumber(string str)
    {
        if (str.size() == 0) return false;

        for (size_t i = 0; i < str.size(); i++)
        {
            if (!isdigit(str[i])) return false;
        }
        return true;
    }

    string readData(std::istream& is)
    {
        char ch = '\0';
        string data;

        while (is >> ch)
        {
            if (ch == '(') break;
        }

        if (is.eof()) return "";

        if (ch != '(') return "";

        getline(is, data, ')');

        return data;
    }

    bool checkData(string data)
    {

        if (data == "end") return false;

        if (data[0] != ':' || data[data.size()-1] != ':') return false;

        if (data.find(":key1 ") == std::string::npos) return false;
        if (data.find(":key2 0") == std::string::npos) return false;
        if (data.find(":key3 \"") == std::string::npos) return false;
        if (data.find("ull:") == std::string::npos) return false;
        if (data.find("\":") == std::string::npos) return false;
        if (data.find("\"") == data.rfind("\":")) return false;

        int key1 = data.find("key1");
        int key2 = data.find("key2");
        int key3 = data.find("key3");
        int ull = data.find("ull");
        int stend = data.rfind("\":");

        if (key1 > ull) return false;

        if (key2 < ull && ull - key2 < ull - key1) return false;

        if (key3 < ull && ull - key3 < ull - key1) return false;

        if (key3 > stend) return false;

        if (key1 < stend && stend - key1 < stend - key3) return false;

        if (key2 < stend && stend - key2 < stend - key3) return false;

        return true;
    }

public:
    bool operator>(DataStruct& other) const
    {
        return compare(other);
    }

    bool operator<(DataStruct& other) const
    {
        return other.compare(*this);
    }

    DataStruct(unsigned long long key1, unsigned long long key2, string key3)
    : key1_(key1), key2_(key2), key3_(std::move(key3)) {}

    DataStruct(): key1_(0), key2_(0), key3_(""){}

    void print() const
    {
        cout << key1_ << " " << key2_ << " " << key3_ << endl;
    }

    unsigned long long toULL(const string& s)
    {
        unsigned long long result = 0;
        for (char c : s)
        {
            result = result * 10 + (c - '0');
        }
        return result;
    }

        friend std::istream& operator>>(std::istream& is, DataStruct& ds)
    {
        string data = ds.readData(is);
        try
        {
            if (data.empty()) throw std::invalid_argument("incorrect");

            size_t key1st = data.find("key1 ") + 5;
            size_t key2st = data.find("key2 ") + 5;
            size_t key3st = data.find("key3 \"") + 6;
            size_t ull = data.find("ull");
            size_t stend = data.rfind("\":");
            size_t oxend = data.find(":", key2st);

            string key1 = data.substr(key1st, ull - key1st);
            string key2 = data.substr(key2st, oxend - key2st);
            string key3 = data.substr(key3st, stend - key3st);

            if (key1.empty() || key2.empty() || key3.empty()) throw std::invalid_argument("incorrect");

            ds.key1_ = std::stoull(key1);
            ds.key2_ = std::stoull(key2, nullptr, 8);
            ds.key3_ = key3;
        }
        catch (...)
        {
            is.setstate(std::ios_base::failbit);
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const DataStruct& ds)
    {
        os << "(:key1 " << std::dec << ds.key1_ << "ull:key2 0";
        os << std::oct << ds.key2_;
        os << ":key3 \"" << ds.key3_ << "\":)";
        return os;
    }

};

int main()
{
    try
    {
        std::vector<DataStruct> structs;
        std::istream_iterator<DataStruct> begin(std::cin);
        std::istream_iterator<DataStruct> end;

        std::copy(begin, end, std::back_inserter(structs));


        std::sort(structs.begin(), structs.end());

        std::copy(structs.begin(), structs.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
