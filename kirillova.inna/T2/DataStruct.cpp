#include "DataStruct.h"

bool kirillova::compareDataStructs(const kirillova::DataStruct& a, const kirillova::DataStruct& b)
{
    if (a.key1 != b.key1)
    {
        return a.key1 < b.key1;
    }
    else if (a.key2 != b.key2)
    {
        return a.key2 < b.key2;
    }
    else
    {
        return a.key3.size() < b.key3.size();
    };
}

std::string kirillova::changeKeyToBinary(unsigned long long key)
{
    std::string result;
    if (key == 0)
    {
        return "0";
    };

    while (key > 0)
    {
        result.insert(result.begin(), (key % 2) ? '1' : '0');
        key /= 2;
    }

    return result;
}

std::istream& kirillova::operator>>(std::istream& in, Delimiter&& data)
{
    std::istream::sentry sentry(in);

    if (!sentry)
    {
        return in;
    }

    char c = '0';
    in >> c;
    c = std::tolower(c);

    if (in && (c != data.value))
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& kirillova::operator>>(std::istream& in, UllLiteral&& data)
{
    std::istream::sentry sentry(in);

    if (!sentry)
    {
        return in;
    }

    return in >> data.value >> Delimiter{ 'u' } >> Delimiter{ 'l' } >> Delimiter{ 'l' } >> Delimiter{ ':' };
}

std::istream& kirillova::operator>>(std::istream& in, UllBinary&& data)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    char c = 0;
    unsigned long long number = 0;
    in >> Delimiter{ '0' } >> c;

    if (c != 'b' && c != 'B')
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    while (in >> c)
    {
        if (c == '1' || c == '0')
        {
            number = (number << 1) + (c - '0');
        }
        else
        {
            break;
        }
    }

    if (c == ':')
    {
        data.value = number;
    }
    else
    {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& kirillova::operator>>(std::istream& in, String&& data)
{
    std::istream::sentry sentry(in);

    if (!sentry)
    {
        return in;
    }

    std::getline(in >> Delimiter{ '"' }, data.value, '"');
    in >> Delimiter{ ':' };

    return in;
}

std::istream& kirillova::operator>>(std::istream& in, DataStruct& data)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    DataStruct temp;

    in >> Delimiter{ '(' } >> Delimiter{ ':' };

    for (size_t i = 0; i < 3; i++)
    {
        std::string key;
        in >> key;

        if (key == "key1")
        {
            in >> UllLiteral{ temp.key1 };
        }
        else if (key == "key2")
        {
            in >> UllBinary{ temp.key2 };
        }
        else if (key == "key3")
        {
            in >> String{ temp.key3 };
        }
        else
        {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    in >> Delimiter{ ')' };

    if (in)
    {
        data = temp;
    }

    return in;
}

std::ostream& kirillova::operator<<(std::ostream& out, const kirillova::DataStruct& data)
{
    out << "(:key1 " << data.key1 << "ull:" << "key2 0b" << changeKeyToBinary(data.key2) << ":" << "key3 \"" << data.key3 << "\":)";
    return out;
}
