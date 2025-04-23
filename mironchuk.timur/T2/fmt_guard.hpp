#ifndef FMT_GUARD_HPP
#define FMT_GUARD_HPP

#include <ios>

class IOFmtGuard {
public:
    explicit IOFmtGuard(std::basic_ios<char> &stream);

    ~IOFmtGuard();

private:
    std::basic_ios<char> &stream_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::ios::fmtflags fmt_;
};

#endif // FMT_GUARD_HPP
