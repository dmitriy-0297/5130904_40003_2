#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

constexpr const char* ERROR_WRONG_NUMBER_OF_ARGUMENTS = "ERROR: should be 2 arguments!";
constexpr const char* ERROR_OF_OPENNING_FILE = "ERROR: can't open the file!";
constexpr const char* ERROR_OF_READING_FILE = "ERROR: can't read the file!";

class WrongNumberOfArguments : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return ERROR_WRONG_NUMBER_OF_ARGUMENTS;
    };
};

class CantOpenFile : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return ERROR_OF_OPENNING_FILE;
    };
};

class CantReadFile : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return ERROR_OF_READING_FILE;
    };
};

#endif
