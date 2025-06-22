#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
};

struct Polygon {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const {
        if (points.size() != other.points.size()) return false;
        return std::is_permutation(points.begin(), points.end(), other.points.begin());
    }
};

double calculateArea(const Polygon& poly) {
    double area = 0.0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        area += poly.points[i].x * poly.points[j].y;
        area -= poly.points[j].x * poly.points[i].y;
    }
    return std::abs(area) / 2.0;
}

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        size_t numVertices;
        if (!(iss >> numVertices)) continue;

        Polygon poly;
        Point p;
        char c;
        bool valid = true;
        for (size_t i = 0; i < numVertices; ++i) {
            if (!(iss >> c >> p.x >> c >> p.y >> c) || c != ')') {
                valid = false;
                break;
            }
            poly.points.push_back(p);
        }

        if (valid && poly.points.size() == numVertices) {
            polygons.push_back(poly);
        }
    }

    return polygons;
}

void processCommands(std::vector<Polygon>& polygons) {
    std::string command;
    while (std::cin >> command) {
        try {
            if (command == "AREA") {
                std::string subcmd;
                std::cin >> subcmd;
                if (subcmd == "EVEN" || subcmd == "ODD") {
                    bool isOdd = (subcmd == "ODD");
                    auto areaSum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [isOdd](double sum, const Polygon& poly) {
                            bool condition = (poly.points.size() % 2) == isOdd;
                            return sum + (condition ? calculateArea(poly) : 0.0);
                        });
                    std::cout << std::fixed << std::setprecision(1) << areaSum << std::endl;
                }
                else if (subcmd == "MEAN") {
                    if (polygons.empty()) {
                        std::cout << "<INVALID COMMAND>" << std::endl;
                        continue;
                    }
                    auto totalArea = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double sum, const Polygon& poly) {
                            return sum + calculateArea(poly);
                        });
                    std::cout << std::fixed << std::setprecision(1) << totalArea / polygons.size() << std::endl;
                }
                else {
                    size_t numVertices = std::stoul(subcmd);
                    auto areaSum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [numVertices](double sum, const Polygon& poly) {
                            return sum + (poly.points.size() == numVertices ? calculateArea(poly) : 0.0);
                        });
                    std::cout << std::fixed << std::setprecision(1) << areaSum << std::endl;
                }
            }
            else if (command == "MAX") {
                std::string subcmd;
                std::cin >> subcmd;
                if (polygons.empty()) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    continue;
                }
                if (subcmd == "AREA") {
                    auto maxIt = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return calculateArea(a) < calculateArea(b);
                        });
                    std::cout << std::fixed << std::setprecision(1) << calculateArea(*maxIt) << std::endl;
                }
                else if (subcmd == "VERTEXES") {
                    auto maxIt = std::max_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return a.points.size() < b.points.size();
                        });
                    std::cout << maxIt->points.size() << std::endl;
                }
                else {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else if (command == "MIN") {
                std::string subcmd;
                std::cin >> subcmd;
                if (polygons.empty()) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    continue;
                }
                if (subcmd == "AREA") {
                    auto minIt = std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return calculateArea(a) < calculateArea(b);
                        });
                    std::cout << std::fixed << std::setprecision(1) << calculateArea(*minIt) << std::endl;
                }
                else if (subcmd == "VERTEXES") {
                    auto minIt = std::min_element(polygons.begin(), polygons.end(),
                        [](const Polygon& a, const Polygon& b) {
                            return a.points.size() < b.points.size();
                        });
                    std::cout << minIt->points.size() << std::endl;
                }
                else {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            else if (command == "COUNT") {
                std::string subcmd;
                std::cin >> subcmd;
                if (subcmd == "EVEN" || subcmd == "ODD") {
                    bool isOdd = (subcmd == "ODD");
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [isOdd](const Polygon& poly) {
                            return (poly.points.size() % 2) == isOdd;
                        });
                    std::cout << count << std::endl;
                }
                else {
                    size_t numVertices = std::stoul(subcmd);
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [numVertices](const Polygon& poly) {
                            return poly.points.size() == numVertices;
                        });
                    std::cout << count << std::endl;
                }
            }
            else if (command == "PERMS") {
                size_t numVertices;
                std::cin >> numVertices;
                Polygon target;
                char c;
                Point p;
                for (size_t i = 0; i < numVertices; ++i) {
                    std::cin >> c >> p.x >> c >> p.y >> c;
                    target.points.push_back(p);
                }
                std::sort(target.points.begin(), target.points.end());
                size_t count = std::count_if(polygons.begin(), polygons.end(),
                    [&target, numVertices](const Polygon& poly) {
                        if (poly.points.size() != numVertices) return false;
                        Polygon sortedPoly = poly;
                        std::sort(sortedPoly.points.begin(), sortedPoly.points.end());
                        return sortedPoly == target;
                    });
                std::cout << count << std::endl;
            }
            else if (command == "MAXSEQ") {
                size_t numVertices;
                std::cin >> numVertices;
                Polygon target;
                char c;
                Point p;
                for (size_t i = 0; i < numVertices; ++i) {
                    std::cin >> c >> p.x >> c >> p.y >> c;
                    target.points.push_back(p);
                }
                size_t maxCount = 0;
                size_t currentCount = 0;
                for (const auto& poly : polygons) {
                    if (poly == target) {
                        currentCount++;
                        maxCount = std::max(maxCount, currentCount);
                    }
                    else {
                        currentCount = 0;
                    }
                }
                std::cout << maxCount << std::endl;
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
                std::string dummy;
                std::getline(std::cin, dummy);
            }
        }
        catch (...) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto polygons = readPolygons(argv[1]);
    processCommands(polygons);

    return 0;
}