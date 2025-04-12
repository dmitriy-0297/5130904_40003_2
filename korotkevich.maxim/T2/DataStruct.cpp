#include <cmath>
#include <sstream>
#include <iomanip>
#include "DataStruct.h"

namespace krrmaxim
{
std::istream& operator>>(std::istream& in, DataStruct& data)
{
  std::string line;
  bool isFound = false; 
  DataStruct temp;

  while (std::getline(in, line))
  {
    if (line.empty())
    {
      continue;
    }

    if (line.front() != '(' || line.back() != ')')
    {
      continue;
    }

    std::string content = line.substr(1, line.size() - 2);
    std::vector<std::string> parts;
    std::istringstream iss(content);
    std::string part;
    while (std::getline(iss, part, ':'))
    {
      if (!part.empty())
      {
        parts.push_back(part);
      }
    }

    bool gotKey1 = false, gotKey2 = false, gotKey3 = false;
    DataStruct parsed;

    for (std::string t: parts)
    {
      t.erase(0, t.find_first_not_of(" \t"));
      t.erase(t.find_last_not_of(" \t") + 1);

      size_t pos = t.find(' ');
      if (pos == std::string::npos)
      {
        continue;
      }
      std::string key = t.substr(0, pos);
      std::string value = t.substr(pos + 1);

      if (key == "key1") 
      {
        if (!std::regex_match(value, REGEXPATTERNKEY1))
        {
          gotKey1 = false;
          break; 
        }
        try
        {
          parsed.key1 = std::stoull(value, nullptr, 16);
          gotKey1 = true;
        }
        catch (const std::invalid_argument& e)
        {
          gotKey1 = false;
          break;
        }
        catch (const std::out_of_range& e) 
        {
          gotKey1 = false;
          break;
        }
      }
            
      else if (key == "key2") 
      {
        if (!std::regex_match(value, REGEXPATTERNKEY2))
        {
          gotKey2 = false;
          break;
        }
        if (value.size() < 7)
        { 
          gotKey2 = false;
          break;
        }
               
        std::string complexPart = value.substr(3, value.size() - 4);
        std::istringstream complexStream(complexPart);
        double re, im;
        if (!(complexStream >> re >> im))
        {
          gotKey2 = false;
          break;
        }
        parsed.key2 = std::complex<double>(re, im);
        gotKey2 = true;
      }
            
      else if (key == "key3") 
      {
           
        if (!std::regex_match(value, REGEXPATTERNKEY3))
        {
          gotKey3 = false;
          break;
        }      
        parsed.key3 = value.substr(1, value.size() - 2);
        gotKey3 = true;
        }
      }

    if (gotKey1 && gotKey2 && gotKey3) 
    {
      temp = parsed;
      isFound = true;
      break;
    }
   
  }
  if (!isFound)
  {
    in.setstate(std::ios::failbit);
  }
  else
  {
    data = temp;
  }
  return in;
}


std::ostream& operator<<(std::ostream& out, const DataStruct& data)
{
  out << "(";
  out << ":key1 ";
  std::ios_base::fmtflags old_flags = out.flags();
  out << "0x" << std::uppercase << std::hex << data.key1;
  out.flags(old_flags);
  out << ":key2 ";
  out << "#c(" << std::fixed << std::setprecision(1) << data.key2.real() << " " << data.key2.imag() << ")";
  out << ":key3 " << "\"" << data.key3 << "\"";
  out << ":)";
  return out;
}

bool dataStructComparator(const DataStruct& d1, const DataStruct& d2)
{
  if (d1.key1 != d2.key1)
  {
    return d1.key1 < d2.key1;
  }
  if (std::abs(d1.key2) != std::abs(d2.key2))
  {
    return std::abs(d1.key2) < std::abs(d2.key2);
  }
  return d1.key3.size() < d2.key3.size();
}
}