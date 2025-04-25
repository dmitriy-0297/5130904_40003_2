#ifndef FMT_GUARD_HPP
#define FMT_GUARD_HPP

#include <iomanip>

class iofmtguard {
public:
    iofmtguard(std::basic_ios<char> &s);

    ~iofmtguard();

private:
    std::basic_ios<char> &s_;
    char fill_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

#endif

