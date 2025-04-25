#include "DataStruct.hpp"
#include "fmt_guard.hpp"

struct DelimiterIO
{
    char exp;
};

struct UnsignedLongLongIO
{
    unsigned long long& ref;
};

struct ComplexIO
{
    std::complex< double >& ref;
};

struct StringIO
{
    std::string& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char ch = 0;
    in >> ch;
    if (in && (ch != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    in >> DelimiterIO{ '0' }
       >> std::oct >> dest.ref
       >> std::dec >> DelimiterIO{ ':' };
    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    double real = 0.0;
    double image = 0.0;

    in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' } >> real;
    in >> image >> DelimiterIO{')'} >> DelimiterIO{':'};

    if (in)
    {
        dest.ref = std::complex< double >(real, image);
    }

    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    DataStruct input{};
    {
        using sep = DelimiterIO;
        using ull = UnsignedLongLongIO;
        using cmp = ComplexIO;
        using str = StringIO;
        in >> sep{ '(' } >> sep{ ':' };
        for (int i = 0; i < 3; i++)
        {
            std::string temp = "";
            in >> temp;
            if (temp == "key1")
            {
                in >> ull{ input.key1 };
            }
            else if (temp == "key2")
            {
                in >> cmp{ input.key2 };
            }
            else if (temp == "key3")
            {
                in >> str{ input.key3 } >> sep{ ':' };
            }
        }
        in >> sep{ ')' };
    }
    if (in)
    {
        dest = input;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    iofmtguard guard(out);

    out << "(:key1 0" << std::oct << dest.key1 << std::dec;
    out << ":key2 #c(" << std::fixed << std::setprecision(1) << dest.key2.real() << ' ' << dest.key2.imag() << ')';
    out << ":key3 " << '"' << dest.key3 << '"' << ":)";

    return out;
}
