#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Point {
    int x;
    int y;
};

struct Polygon {
    std::vector<Point> points;
};

bool operator==(const Point &a, const Point &b) {
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon &a, const Polygon &b) {
    return a.points == b.points;
}

double area(const Polygon &p) {
    if (p.points.size() < 3) {
        return 0.0;
    }
    auto sum = std::inner_product(p.points.begin(),
                                  std::prev(p.points.end()),
                                  std::next(p.points.begin()),
                                  0LL,
                                  std::plus<>(),
                                  [](const Point &lhs, const Point &rhs) {
                                      return static_cast<long long>(lhs.x) * rhs.y -
                                             static_cast<long long>(rhs.x) * lhs.y;
                                  });
    sum += static_cast<long long>(p.points.back().x) * p.points.front().y -
            static_cast<long long>(p.points.front().x) * p.points.back().y;
    return std::abs(sum) * 0.5;
}

bool parsePoint(std::istream &in, Point &pt) {
    char c = 0;
    if (!(in >> c) || c != '(') {
        return false;
    }
    if (!(in >> pt.x)) {
        return false;
    }
    if (!(in >> c) || c != ';') {
        return false;
    }
    if (!(in >> pt.y)) {
        return false;
    }
    if (!(in >> c) || c != ')') {
        return false;
    }
    return true;
}

bool parsePolygon(std::istream &in, Polygon &poly) {
    int n = 0;
    if (!(in >> n) || n < 3) {
        return false;
    }
    Polygon tmp;
    tmp.points.reserve(n);
    for (int i = 0; i < n; ++i) {
        Point pt{};
        if (!parsePoint(in, pt)) {
            return false;
        }
        tmp.points.push_back(pt);
    }
    poly = std::move(tmp);
    return true;
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    if (argc < 2) {
        std::cerr << "filename required\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "cannot open file\n";
        return 1;
    }
    std::vector<Polygon> figures;
    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        Polygon p;
        if (parsePolygon(iss, p)) {
            figures.push_back(std::move(p));
        }
    }
    std::cout << std::fixed << std::setprecision(1);
    while (std::getline(std::cin, line)) {
        std::istringstream cmd(line);
        std::string op;
        if (!(cmd >> op)) {
            std::cout << "<INVALID COMMAND>\n";
            continue;
        }
        if (op == "AREA") {
            std::string arg;
            if (!(cmd >> arg)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (arg == "EVEN" || arg == "ODD") {
                bool even = (arg == "EVEN");
                double res = std::accumulate(figures.begin(),
                                             figures.end(),
                                             0.0,
                                             [even](double acc, const Polygon &p) {
                                                 return ((p.points.size() % 2 == 0) == even)
                                                            ? acc + area(p)
                                                            : acc;
                                             });
                std::cout << res << "\n";
            } else if (arg == "MEAN") {
                if (figures.empty()) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                double res = std::accumulate(figures.begin(),
                                             figures.end(),
                                             0.0,
                                             [](double acc, const Polygon &p) { return acc + area(p); }) /
                             figures.size();
                std::cout << res << "\n";
            } else {
                int v = 0;
                try {
                    v = std::stoi(arg);
                } catch (...) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                double res = std::accumulate(figures.begin(),
                                             figures.end(),
                                             0.0,
                                             [v](double acc, const Polygon &p) {
                                                 return (static_cast<int>(p.points.size()) == v)
                                                            ? acc + area(p)
                                                            : acc;
                                             });
                std::cout << res << "\n";
            }
        } else if (op == "MAX" || op == "MIN") {
            std::string arg;
            if (!(cmd >> arg) || figures.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (arg == "AREA") {
                auto it = (op == "MAX")
                              ? std::max_element(figures.begin(),
                                                 figures.end(),
                                                 [](const Polygon &a, const Polygon &b) { return area(a) < area(b); })
                              : std::min_element(figures.begin(),
                                                 figures.end(),
                                                 [](const Polygon &a, const Polygon &b) { return area(a) < area(b); });
                std::cout << area(*it) << "\n";
            } else if (arg == "VERTEXES") {
                auto getSize = [](const Polygon &p) { return p.points.size(); };
                auto it = (op == "MAX")
                              ? std::max_element(figures.begin(),
                                                 figures.end(),
                                                 [&](const Polygon &a, const Polygon &b) {
                                                     return getSize(a) < getSize(b);
                                                 })
                              : std::min_element(figures.begin(),
                                                 figures.end(),
                                                 [&](const Polygon &a, const Polygon &b) {
                                                     return getSize(a) < getSize(b);
                                                 });
                std::cout << it->points.size() << "\n";
            } else {
                std::cout << "<INVALID COMMAND>\n";
            }
        } else if (op == "COUNT") {
            std::string arg;
            if (!(cmd >> arg)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (arg == "EVEN" || arg == "ODD") {
                bool even = (arg == "EVEN");
                size_t c = std::count_if(figures.begin(),
                                         figures.end(),
                                         [even](const Polygon &p) { return (p.points.size() % 2 == 0) == even; });
                std::cout << c << "\n";
            } else {
                int v = 0;
                try {
                    v = std::stoi(arg);
                } catch (...) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                size_t c = std::count_if(figures.begin(),
                                         figures.end(),
                                         [v](const Polygon &p) { return static_cast<int>(p.points.size()) == v; });
                std::cout << c << "\n";
            }
        } else if (op == "ECHO") {
            Polygon target;
            if (!parsePolygon(cmd, target)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            std::vector<Polygon> out;
            out.reserve(figures.size() * 2);
            size_t added = 0;
            std::for_each(figures.begin(),
                          figures.end(),
                          [&](const Polygon &p) {
                              out.push_back(p);
                              if (p == target) {
                                  out.push_back(p);
                                  ++added;
                              }
                          });
            figures.swap(out);
            std::cout << added << "\n";
        } else if (op == "MAXSEQ") {
            Polygon target;
            if (!parsePolygon(cmd, target)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            size_t best = 0;
            size_t cur = 0;
            std::for_each(figures.begin(),
                          figures.end(),
                          [&](const Polygon &p) {
                              if (p == target) {
                                  ++cur;
                                  best = std::max(best, cur);
                              } else {
                                  cur = 0;
                              }
                          });
            std::cout << best << "\n";
        } else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}
