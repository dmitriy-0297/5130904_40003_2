#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <iterator>
#include <sstream>

const char* COMPARING_ERROR = "There are equal structures";
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

        throw std::runtime_error(COMPARING_ERROR);
    }

    bool isNumber(string str)
    {
        if (str.size() == 0) return false;

        for (int i = 0; i < str.size(); i++)
        {
            if (!isdigit(str[i])) return false;
        }
        return true;
    }

    string readData(std::istream& is)
    {
        char ch;
        string data;

        while (is >> ch && ch != '(') { }

        if (ch != '(')
        {
            data = "\0";
            return data;
        }

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

    friend std::istream& operator>>(std::istream& is, DataStruct& ds)
    {
        string data = ds.readData(is);
        if (!ds.checkData(data))
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        int key1st = data.find("key1 ") + 5;
        int key2st = data.find("key2 0") + 6;
        int key3st = data.find("key3 \"") + 6;
        int ull = data.find("ull");
        int stend = data.rfind("\":");
        int oxend = data.find(":", key2st);

        string key1 = "";
        string key2 = "";
        string key3 = "";

        for (int i = key1st; i < ull; i++) key1.push_back(data[i]);
        for (int i = key2st; i < oxend; i++) key2.push_back(data[i]);
        for (int i = key3st; i < stend; i++) key3.push_back(data[i]);

        if (key3.size() == 0) return is;

        if (!ds.isNumber(key1) || !ds.isNumber(key2)) return is;

        ds.key1_ = std::stoull(key1);
        ds.key2_ = std::stoull(key2, nullptr, 8);
        ds.key3_ = key3;

        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const DataStruct& ds)
    {
        os << "(:key1 " << ds.key1_ << "ull:key2 0";
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
