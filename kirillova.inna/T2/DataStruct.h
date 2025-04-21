#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>

namespace kirillova
{
	struct DataStruct
	{
		unsigned long long key1;
		unsigned long long key2;
		std::string key3;

		DataStruct() : key1(0), key2(0), key3("") {};
	};

	bool compareDataStructs(const DataStruct& a, const DataStruct& b);
	std::string changeKeyToBinary(unsigned long long key);

	struct Delimiter
	{
		char value;
	};

	struct UllLiteral
	{
		unsigned long long& value;
	};

	struct UllBinary
	{
		unsigned long long& value;
	};

	struct String
	{
		std::string& value;
	};

	std::istream& operator>>(std::istream& in, DataStruct& data);
	std::istream& operator>>(std::istream& in, Delimiter&& data);
	std::istream& operator>>(std::istream& in, UllLiteral&& data);
	std::istream& operator>>(std::istream& in, UllBinary&& data);
	std::istream& operator>>(std::istream& in, String&& data);
	std::ostream& operator<<(std::ostream& out, const DataStruct& data);
}

#endif
