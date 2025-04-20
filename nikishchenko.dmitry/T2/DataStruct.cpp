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

std::istream& operator>>(std::istream& in, SeparatorIO&& sep)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char inputValue = '0';
    in >> inputValue;
    if (in && (inputValue != sep.exp))
    {
        in.setstate(std::ios_base::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, LabelIO& sep)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    std::string label = "";
    in >> label;
    bool invalidInputMask = in.eof() || label.length() != LABEL_LENGTH
        || label.substr(0, 3) != "key" || !(std::isdigit(label.c_str()[3]));
    if (invalidInputMask)
    {
        in.setstate(std::ios_base::failbit);
        return in;
    }
    sep.exp = label;
    return in;
}

void ignoreInput(std::istream& in)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        in.clear(std::ios_base::goodbit);
    }
    const char skipToVal = ')';
    char input = '0';
    in >> input;
    if (input == '(')
    {
        in.putback('(');
        return;
    }
    while (!(in.eof() || input == skipToVal))
    {
        in >> input;
    }
}

std::string getCorrectRound(const double& num)
{
    std::string intPart = std::to_string(static_cast<int>(num));

    int doublePartNum = std::round((100 * num));
    doublePartNum %= 100;
    std::string doublePart = "";
    if (doublePartNum > 0 && doublePartNum < 10)
    {
        doublePart = "0" + std::to_string(doublePartNum);
    }
    else if (doublePartNum % 10 == 0)
    {
        doublePart = std::to_string(doublePartNum/10);
    }
    else
    {
        doublePart = std::to_string(doublePartNum);
    }

    return intPart + "." + doublePart;
}

std::istream& operator>>(std::istream& in, DataStruct& destination)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    double key1 = 0.0;
    long long key2 = 0ll;
    std::string key3 = "";
    {
        using sep = SeparatorIO;
        LabelIO label{ "" };
        int iter = 0;
        int option = 0;
        bool stateFlag = true;
        bool regexMaskSLL = true;
        bool regexMaskDouble = true;
        bool regexMaskInt = true;
        std::string inputVal{ "" };

        if (!(in >> sep{ '(' } >> sep{ ':' }))
        {
            stateFlag = false;
        }
        while (in && iter < 3 && stateFlag)
        {
            in >> label;
            in >> std::ws;
            if (!in)
            {
                stateFlag = false;
                break;
            }
            option = static_cast<int>(label.exp[3]);
            switch (option)
            {
            case OPTION_ONE:
                std::getline(in, inputVal, ':');
                regexMaskDouble = std::regex_match(inputVal, std::regex(REGEX_DBL_LIT_DOUBLE));
                regexMaskInt =  std::regex_match(inputVal, std::regex(REGEX_DBL_LIT_INT));
                if (regexMaskDouble || regexMaskInt)
                {
                    key1 = std::stod(inputVal);
                }
                else
                {
                    stateFlag = false;
                }
                break;
            case OPTION_TWO:
                std::getline(in, inputVal, ':');
                regexMaskSLL = std::regex_match(inputVal, std::regex(REGEX_SLL));
                if (regexMaskSLL)
                {
                    key2 = std::stoll(inputVal);
                }
                else
                {
                    stateFlag = false;
                }
                break;
            case OPTION_THREE:
                in >> sep{ '\"' };
                std::getline(in, inputVal, '\"');
                in >> sep{ ':' };
                if (in)
                {
                    key3 = inputVal;
                }
                else
                {
                    stateFlag = false;
                }
                break;
            default:
                stateFlag = false;
                break;
            }
            inputVal = "";
            iter++;
        }

        if (in && stateFlag && iter == 3)
        {
            destination.key1 = key1;
            destination.key2 = key2;
            destination.key3 = key3;
            in >> sep{ ')' };
        }
        else
        {
            ignoreInput(in);
            destination.key1 = INVALID_DOUBLE_INDICATOR;
            destination.key2 = INVALID_LL_INDICATOR;
            destination.key3 = INVALID_STR_INDICATOR;
        }
        return in;
    }
}


std::ostream& operator<<(std::ostream& out, const DataStruct& dataStruct)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    StreamGuard streamGuard(out);
    out << "(:key1 "
        << getCorrectRound(dataStruct.key1) << "d"
        << ":key2 "
        << dataStruct.key2 << "ll"
        << ":key3 \""
        << dataStruct.key3 << "\""
        << ":)";
    return out;
}

bool compareDataStruct(const DataStruct& first, const DataStruct& second) {
    if (first.key1 < second.key1)
    {
        return true;
    }
    else if (first.key1 == second.key1)
    {
        if (first.key2 < second.key2)
        {
            return true;
        }
        else if (first.key2 == second.key2)
        {
            return first.key3.length() < second.key3.length();
        }
    }
    return false;
}
