#include "DataStruct.h"
#include <cstring>

#define KEY_WORD "key"
#define VALUE_OF_KEY_1 "'*'"
#define VALUE_OF_KEY_2_FIRST "(:N "
#define VALUE_OF_KEY_2_SECOND ":D "
#define END_OF_KEY_2 ":)"
#define SIZE_OF_STRING 32

std::ostream& operator<< (std::ostream& out, const DataStruct& data)
{
  out << "(:key1 '" << data.key1
    << "':key2 (:N " << data.key2.first
    << ":D " << data.key2.second
    << ":):key3 \"" << data.key3 << "\":)";

  return out;
}

std::istream& operator>> (std::istream& in, DataStruct& data)
{
  char symb = ' ';
  char* str = new char[SIZE_OF_STRING];
  int numberOfKey = 0;
  bool isCorrect = true;

  char key1 = ' ';
  std::pair<long long, unsigned long long> key2 = { 0, 0 };
  std::string key3 = "";

  while (symb != '\n' && !in.eof())
  {
    in.get(symb);

    if (symb == '(' && in.peek() != ':')
    {
      isCorrect = false;
    }

    if (symb == ':' && in.peek() != ')')
    {
      in.get(str, std::streamsize(sizeof(KEY_WORD)));

      if (strcmp(str, KEY_WORD) == 0)
      {
        in.get(symb);
        numberOfKey = static_cast< int >(symb) - static_cast< int >('0');
        in.get();

        switch (numberOfKey)
        {
        case(1):
          in.get(str, std::streamsize(sizeof(VALUE_OF_KEY_1)));

          if (str[0] == '\'' && str[2] == '\'')
          {
            key1 = str[1];
          }
          else
          {
            isCorrect = false;
          }
          break;

        case(2):
          in.get(str, std::streamsize(sizeof(VALUE_OF_KEY_2_FIRST)));
          if (strcmp(str, VALUE_OF_KEY_2_FIRST) == 0)
          {
            in >> key2.first;
            if (in.fail())
            {
              in.clear();
              isCorrect = false;
              break;
            }
          }

          in.get(str, std::streamsize(sizeof(VALUE_OF_KEY_2_SECOND)));
          if (strcmp(str, VALUE_OF_KEY_2_SECOND) == 0)
          {
            in >> key2.second;
            if (in.fail())
            {
              in.clear();
              isCorrect = false;
              break;
            }
          }

          in.get(str, std::streamsize(sizeof(END_OF_KEY_2)));
          if (strcmp(str, END_OF_KEY_2) != 0)
          {
            isCorrect = false;
            break;
          }
          break;

        case(3):
          in.get(symb);
          if (symb == '"')
          {
            bool isClosed = false;
            while (!isClosed && in.peek() != '\n')
            {
              if (in.peek() == '"')
              {
                isClosed = true;
                break;
              }

              in.get(symb);
              key3 += symb;
            }

            if (!isClosed)
            {
              isCorrect = false;
              break;
            }
          }
          else
          {
            isCorrect = false;
          }
          break;

        default:
          isCorrect = false;
          break;
        }
      }
    }

    if(!isCorrect)
    {
      numberOfKey = 0;
      isCorrect = true;
      key1 = ' ';
      key2 = { 0, 0 };
      key3 = "";

      while (symb != '\n')
      {
        in.get(symb);
      }
      symb = ' ';
    }

  }

  if (isCorrect)
  {
    data.key1 = key1;
    data.key2 = key2;
    data.key3 = key3;
  }

  delete[] str;
  return in;
}

