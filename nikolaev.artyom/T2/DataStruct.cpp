#include <iomanip>
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
      while (stream.get(next_char))
      {
        if (next_char == ':' || next_char == ')')
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
        if (isString(value))
        {
          temp.key3 = value;
          found3 = true;
        }
        else
        {
          parsingSuccess = false;
        }
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

  output << "(:key1 "  << std::setprecision(1) << std::scientific << data.key1
    << ":key2 " << data.key2 << "ll"
    << ":key3 " << data.key3 << ":)\n";

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
