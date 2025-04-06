#include <cmath>
#include <sstream>
#include "DataStruct.h"
#include "StreamGuard.h"

namespace artttnik
{
bool isDouble(const std::string &str)
{
  return std::regex_match(str, PATTERNFORKEY1);
}

bool isLLong(const std::string &str)
{
  return std::regex_match(str, PATTERNFORKEY2);
}

bool isString(const std::string &str)
{
  return std::regex_match(str, PATTERNFORKEY3);
}

std::string makeScientific(const double &data)
{
  if (data == 0.0)
  {
    return "0.0e0";
  }

  double absData = std::abs(data);
  int exponent = static_cast<int>(std::floor(std::log10(absData)));
  double mantissa = absData / std::pow(10.0, exponent);

  if (mantissa >= 10.0)
  {
    mantissa /= 10.0;
    exponent++;
  }
  else if (mantissa < 1.0)
  {
    mantissa *= 10.0;
    exponent--;
  }

  mantissa = std::round(mantissa * 10.0) / 10.0;
  if (mantissa == 10.0)
  {
    mantissa = 1.0;
    exponent++;
  }

  std::string result;
  if (data < 0)
  {
    result += '-';
  }
  std::string mantissaStr = std::to_string(mantissa);
  size_t dotPos = mantissaStr.find('.');
  if (dotPos != std::string::npos)
  {
    mantissaStr = mantissaStr.substr(0, dotPos + 2);
  }

  if (mantissaStr.back() == '0')
  {
    mantissaStr.erase(mantissaStr.find('.') + 2, std::string::npos);
  }

  if (mantissaStr.back() == '.')
  {
    mantissaStr += '0';
  }

  result += mantissaStr;
  result += 'e';

  if (exponent >= 0)
  {
    result += '+';
  }
  result += std::to_string(exponent);

  return result;
}

std::istream &operator>>(std::istream &input, DataStruct &data)
{
  std::string line;
  while (std::getline(input, line))
  {
    std::istringstream stream(line);
    char ch;

    if (!(stream >> ch) || ch != '(')
    {
      continue;
    }

    DataStruct temp;
    bool found1 = false, found2 = false, found3 = false;
    bool parsingSuccess = true;

    while (parsingSuccess)
    {
      std::string key;
      if (!(stream >> key) || key.empty() || key[0] != ':')
      {
        parsingSuccess = false;
        break;
      }

      std::string value;
      char next_char;
      bool inQuotes = false;

      while (stream.get(next_char))
      {
        if (key == ":key3" && next_char == '"')
        {
          inQuotes = !inQuotes;
        }

        if (!inQuotes && (next_char == ':' || next_char == ')'))
        {
          stream.unget();
          break;
        }

        value += next_char;
      }

      value.erase(0, value.find_first_not_of(" \t"));
      value.erase(value.find_last_not_of(" \t") + 1);

      if (key == ":key1")
      {
        if (isDouble(value))
        {
          temp.key1 = std::stod(value);
          found1 = true;
        }
        else
        {
          parsingSuccess = false;
        }
      }
      else if (key == ":key2")
      {
        if (isLLong(value))
        {
          temp.key2 = std::stoll(value);
          found2 = true;
        }
        else
        {
          parsingSuccess = false;
        }
      }
      else if (key == ":key3")
      {
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"')
        {
          value = value.substr(1, value.size() - 2);
        }
        temp.key3 = value;
        found3 = true;
      }
      else if (key == ":)")
      {
        break;
      }
      else
      {
        parsingSuccess = false;
      }

      if (stream.peek() == ')')
      {
        stream.get();
        break;
      }
    }

    if (parsingSuccess && found1 && found2 && found3)
    {
      data = std::move(temp);
      return input;
    }
  }

  input.setstate(std::ios::eofbit);
  return input;
}

std::ostream &operator<<(std::ostream &output, const DataStruct &data)
{
  const std::ostream::sentry sentry(output);
  if (!sentry)
  {
    return output;
  }

  StreamGuard guard(output);

  output << "(:key1 "  << makeScientific(data.key1)
    << ":key2 " << data.key2 << "ll"
    << ":key3 \"" << data.key3 << "\":)\n";

  return output;
}

bool compareDataStruct(const DataStruct &data1, const DataStruct &data2)
{
  if (data1.key1 != data2.key1)
  {
    return data1.key1 < data2.key1;
  }

  if (data1.key2 != data2.key2)
  {
    return data1.key2 < data2.key2;
  }

  return data1.key3 < data2.key3;
}
}
