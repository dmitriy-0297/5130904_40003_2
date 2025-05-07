#include "DataStruct.h"

StreamGuard::StreamGuard(std::basic_ios<char>& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{}

StreamGuard::~StreamGuard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}

std::complex<double> parse_complex(const std::string& input)
{
    std::complex<double> num;
    double real, imag;
    std::string inner = input.substr(3, input.length() - 4);
    std::istringstream iss(inner);
    iss >> real >> imag;
    return std::complex<double>(real, imag);
}

std::istream& operator>>(std::istream& in, DelimiterIO&& data)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    char c = '0';
    in >> c;
    if (in && c != data.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, LabelIO& data)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string s = "";
    in >> s;
    bool invalidInput = in.eof() || s.length() != LENGTH_OF_LABEL;
    if (!invalidInput) {
        invalidInput = s.substr(0, 3) != "key" || !std::isdigit(s[3]);
    }

    if (invalidInput) {
        in.setstate(std::ios::failbit);
        return in;
    }

    data.exp = s;
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& data)
{
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    long long key1 = 0ll;
    std::complex<double> key2;
    std::string key3;
    DataStruct temp;

    char c;
    using sep = DelimiterIO;
    LabelIO label{ "" };
    int iter = 0;
    int option = 0;
    bool streamFlag = true;
    bool sllMask = true;
    bool cmpMask = true;
    std::string inputStr = "";

    if (!in || in.peek() == EOF) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (!(in >> sep{ '(' } && in >> sep{ ':' })) {
        streamFlag = false;
    }

    while (in && iter < 3 && streamFlag) {
        in >> label;
        in >> std::ws;
        option = static_cast<int>(label.exp[3]) - 48;

        switch (option) {
        case 1:
            std::getline(in, inputStr, ':');
            if (std::regex_match(inputStr, REGEX_SLL_LIT)) {
                temp.key1 = std::stoll(inputStr);
            }
            else {
                sllMask = false;
            }
            break;

        case 2:
            std::getline(in, inputStr, ':');
            if (std::regex_match(inputStr, REGEX_CMP_LSP)) {
                temp.key2 = parse_complex(inputStr);
            }
            else {
                cmpMask = false;
            }
            break;

        case 3:
            in >> sep{ '\"' };
            std::getline(in, inputStr, '\"');
            in >> sep{ ':' };
            if (in) {
                temp.key3 = inputStr;
            }
            else {
                streamFlag = false;
            }
            break;

        default:
            streamFlag = false;
            break;
        }

        inputStr = "";
        iter++;
    }

    if (in && streamFlag && sllMask && cmpMask && iter == 3 && (in >> sep{ ')' })) {
        data = temp;
        return in;
    }

    in.setstate(std::ios::failbit);
    in.clear();
    in >> c;
    while (in && c != ')') {
        if (in.peek() == EOF) {
            return in;
        }
        in >> c;
    }

    return operator>>(in, data);
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
    StreamGuard guard(out);
    out << "(:"
        << "key1 " << data.key1 << "ll:"
        << "key2 #c(" << std::fixed << std::setprecision(1)
        << data.key2.real() << " " << data.key2.imag() << "):"
        << "key3 \"" << data.key3 << "\":)";
    return out;
}

bool compare(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }

    if (std::abs(a.key2) != std::abs(b.key2)) {
        return std::abs(a.key2) < std::abs(b.key2);
    }

    return a.key3.length() < b.key3.length();
}
