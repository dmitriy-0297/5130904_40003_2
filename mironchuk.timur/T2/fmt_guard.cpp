#include "fmt_guard.hpp"

IOFmtGuard::IOFmtGuard(std::basic_ios<char> &s) : stream_(s),
                                                  width_(s.width()),
                                                  fill_(s.fill()),
                                                  precision_(s.precision()),
                                                  fmt_(s.flags()) {
}

IOFmtGuard::~IOFmtGuard() {
    stream_.width(width_);
    stream_.fill(fill_);
    stream_.precision(precision_);
    stream_.flags(fmt_);
}
