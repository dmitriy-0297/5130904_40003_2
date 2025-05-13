#include <iostream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <numeric>
#include <map>
#include <unordered_set>

namespace utilities
{
    enum CommandOptions
    {
        NOT_FOUND = -1,
        AREA = -2,
        ODD = -3,
        EVEN = -4,
        MEAN = -5,
        MAX = -6,
        MIN = -7,
        VERTEXES = -8,
        COUNT = -9,
        RMECHO = -10,
        SAME = -11
    };

    const std::map<std::string, int> COMMAND_MAP = {
        {"AREA", AREA},
        {"ODD", ODD},
        {"EVEN", EVEN},
        {"MEAN", MEAN},
        {"MAX", MAX},
        {"MIN", MIN},
        {"VERTEXES", VERTEXES},
        {"COUNT", COUNT},
        {"RMECHO", RMECHO},
        {"SAME", SAME}
    };

    int getOption(std::string& option)
    {
        auto it = COMMAND_MAP.find(option);
        return (it != COMMAND_MAP.end()) ? it->second : NOT_FOUND;
    }

    const std::string FILE_NAME = "filename.txt";
    const std::string BAD_COMMAND = "<INVALID COMMAND>";

    class StreamGuard
    {
    public:
        StreamGuard(std::basic_ios<char>& s) : s_(s), precision_(s.precision()), fmt_(s.flags()) {};
        ~StreamGuard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize precision_;
        std::basic_ios<double>::fmtflags fmt_;
    };

    StreamGuard::~StreamGuard()
    {
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}

namespace geometry
{
    struct Point
    {
        int x, y;

        Point(int inX, int inY) : x(inX), y(inY) {};
        Point(const Point& p);
        Point& operator=(const Point& p);
        Point(Point&& p) noexcept;
        Point& operator=(Point&& p) noexcept;
        ~Point() = default;

        bool operator==(const Point& otherPoint) const
        {
            return (x == otherPoint.x) && (y == otherPoint.y);
        }
    };

    bool inputPoints(std::istream& in, std::vector<Point>& points, int& numOfPoints);

    struct Polygon
    {
        std::vector<Point> points;
        bool operator==(const Polygon& otherPoly) const;

        Polygon() = default;
        ~Polygon() = default;
        Polygon(const Polygon& poly);
        Polygon& operator=(const Polygon& poly);
        Polygon(Polygon&& poly) noexcept;
        Polygon& operator=(Polygon&& poly) noexcept;

    };
    std::istream& operator>>(std::istream& in, Polygon& poly);

    double countShoelaceArea(const Polygon& poly);

    struct OddOrEvenDefiner
    {
        OddOrEvenDefiner(bool odd = true) : odd_(odd) {}
        double operator()(double area, const Polygon& poly) const
        {
            bool isOdd = (poly.points.size() % 2 != 0);
            area += (odd_ == isOdd) ? countShoelaceArea(poly) : 0.0;
            return area;
        }

    private:
        bool odd_;
    };
}

using namespace geometry;
using namespace utilities;
using namespace std::placeholders;

namespace same
{
    struct pairHash {
        std::size_t operator()(const std::pair<int, int>& p) const {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };

    using vecArr = std::unordered_set<std::pair<int, int>, pairHash>;

    vecArr computeVectors(const std::vector<Point>& points, bool reverse);

    struct VecComparator
    {
        size_t targetSize;
        vecArr targetVecs;
        vecArr negTargetVecs;

        bool operator()(const Polygon& poly) const
        {
            if (poly.points.size() != targetSize)
            {
                return false;
            }

            vecArr polyVecs = computeVectors(poly.points, false);
            vecArr polyNegVecs = computeVectors(poly.points, true);

            return (polyVecs == targetVecs) || (polyNegVecs == targetVecs);
        }
    };

    int countSame(const std::vector<Polygon>& figures, const Polygon& targetPoly)
    {
        size_t targetSize = targetPoly.points.size();
        vecArr targetVecs = computeVectors(targetPoly.points, false);
        vecArr targetNegVecs = computeVectors(targetPoly.points, true);

        int count = std::count_if(figures.cbegin(), figures.cend(), VecComparator{targetSize, targetVecs, targetNegVecs});
        return count;
    }
}

using namespace same;

template<typename T>
bool checkOpenFile(T& stream);

void input(std::ifstream& fileInput, std::vector<Polygon>& figures);

double countArea(const std::vector<Polygon>& figures, const int& option);
void maxAreaVertexes(const std::vector<Polygon>& figures, const int& option, std::ostream& out);
void minAreaVertexes(const std::vector<Polygon>& figures, const int& option, std::ostream& out);
int countFigures(const std::vector<Polygon>& figures, const int& option);
int deleteFigures(std::vector<Polygon>& figures, const Polygon& poly);
void commandLine(std::string& stream, std::vector<Polygon>& figures, std::string& option, int& opt);




int main(int args, char* argVector[])
{
    if (args != 2)
    {
        return EXIT_FAILURE;
    }
    std::ifstream fileInput(argVector[1]);

    if (!fileInput.is_open())
    {
        return EXIT_FAILURE;
    }

    std::vector<Polygon> figures;
    std::string option;
    std::string stream;
    int opt = 0;

    input(fileInput, figures);
    commandLine(stream, figures, option, opt);

    return EXIT_SUCCESS;
}




template<typename T>
bool checkOpenFile(T& stream)
{
    return !(stream.fail());
}

bool geometry::inputPoints(std::istream& in, std::vector<Point>& points, int& numOfPoints)
{
    if (numOfPoints < 1 || in.fail())
    {
        return !(in.fail());
    }

    char input{ '\0' };
    int tempX = 0, tempY = 0;
    bool mask = (in >> input && input == '(')
        && (in >> tempX) && (in >> input && input == ';')
        && (in >> tempY) && (in >> input && input == ')');
    if (!mask)
    {
        in.setstate(std::ios_base::failbit);
    }
    else
    {
        points.push_back(Point(tempX, tempY));
    }
    return inputPoints(in, points, --numOfPoints);
}

void commandLine(std::string& stream, std::vector<Polygon>& figures, std::string& option, int& opt)
{
    if (std::getline(std::cin, stream))
    {
        std::istringstream iss(stream);
        iss >> option;
        opt = getOption(option);

        switch (opt)
        {
        case AREA:
            iss >> option;
            {
                StreamGuard stream(std::cout);
                std::cout << std::fixed << std::setprecision(1);
                try
                {
                    if (std::stoi(option) && std::stoi(option) >= 3)
                    {
                        std::cout << countArea(figures, std::stoi(option)) << '\n';
                        break;
                    }
                    else
                    {
                        std::cout << BAD_COMMAND << '\n';
                        break;
                    }
                }
                catch (std::invalid_argument& notNumber)
                {
                    opt = getOption(option);

                    switch (opt)
                    {
                    case EVEN:
                        std::cout << countArea(figures, EVEN) << '\n';
                        break;
                    case ODD:
                        std::cout << countArea(figures, ODD) << '\n';
                        break;
                    case MEAN:
                        std::cout << countArea(figures, MEAN) << '\n';
                        break;
                    default:
                        std::cout << BAD_COMMAND << '\n';
                        break;
                    }
                }
            }
            break;

        case MAX:
            iss >> option;
            opt = getOption(option);

            if (opt == AREA || opt == VERTEXES)
            {
                maxAreaVertexes(figures, opt, std::cout);
                break;
            }
            std::cout << BAD_COMMAND << '\n';
            break;

        case MIN:
            iss >> option;
            opt = getOption(option);

            if (opt == AREA || opt == VERTEXES)
            {
                minAreaVertexes(figures, opt, std::cout);
                break;
            }
            break;

        case COUNT:
            iss >> option;
            try
            {
                if (std::stoi(option) && std::stoi(option) >= 3)
                {
                    std::cout << countFigures(figures, std::stoi(option)) << '\n';
                    break;
                }
                else
                {
                    std::cout << BAD_COMMAND << '\n';
                    break;
                }
            }
            catch (std::invalid_argument& notNumber)
            {
                opt = getOption(option);
                switch (opt)
                {
                case EVEN:
                    std::cout << countFigures(figures, EVEN) << '\n';
                    break;
                case ODD:
                    std::cout << countFigures(figures, ODD) << '\n';
                    break;
                default:
                    std::cout << BAD_COMMAND << '\n';
                    break;
                }
            }
            break;

        case RMECHO:
        {
            Polygon targetPoly;
            if (!(iss >> targetPoly).eof() || targetPoly.points.empty())
            {
                std::cout << BAD_COMMAND << '\n';
                break;
            }

            std::cout << deleteFigures(figures, targetPoly) << '\n';
            break;
        }

        case SAME:
        {
            Polygon targetPoly;
            if (!(iss >> targetPoly).eof() || targetPoly.points.empty())
            {
                std::cout << BAD_COMMAND << '\n';
                break;
            }

            std::cout << same::countSame(figures, targetPoly) << '\n';
            break;
        }

        case NOT_FOUND:
            std::cout << BAD_COMMAND << '\n';
        }

        commandLine(stream, figures, option, opt);
    }
}


std::istream& geometry::operator>>(std::istream& in, Polygon& poly)
{
    int numOfPoints = 0;
    std::string end;
    bool correctInputMask = (in >> numOfPoints) && (numOfPoints >= 3) && inputPoints(in, poly.points, numOfPoints) && !(in >> end);
    if (!correctInputMask)
    {
        poly.points.clear();
    }
    return in;
}

void input(std::ifstream& fileInput, std::vector<Polygon>& figures)
{
    static std::string line;
    static Polygon inPoly;

    if (std::getline(fileInput, line))
    {
        std::istringstream iss(line);
        if ((iss >> inPoly).eof())
        {
            figures.push_back(std::move(inPoly));
        }
        input(fileInput, figures);
    }
    else
    {
        figures.erase(std::remove_if(figures.begin(), figures.end(),
            [](Polygon& poly) { return poly.points.empty(); }), figures.end());
    }
}

double geometry::countShoelaceArea(const Polygon& poly)
{
    double totalArea = 0.0;
    if (!poly.points.empty())
    {
        double initArea = (poly.points.back().x * poly.points.front().y) - (poly.points.back().y * poly.points.front().x);
        auto countPairArea = [prev = poly.points.front()](double area, const Point& current) mutable
            {
                area += ((prev.x * current.y) - (prev.y * current.x));
                prev = current;
                return area;

            };

        totalArea = std::accumulate(poly.points.cbegin(), poly.points.cend(), initArea, countPairArea);
    }
    return std::abs(totalArea)/2;
}

double countArea(const std::vector<Polygon>& figures, const int& option)
{
    double area = 0.0;
    switch (option)
    {
    case ODD:
        area = std::accumulate(figures.cbegin(), figures.cend(), 0.0, OddOrEvenDefiner(true));
        return area;
    case EVEN:
        area = std::accumulate(figures.cbegin(), figures.cend(), 0.0, OddOrEvenDefiner(false));
        return area;
    case MEAN:
        area = std::accumulate(figures.cbegin(), figures.cend(), 0.0, [](double acc, const Polygon& poly) {
            return acc + countShoelaceArea(poly);
            }) / figures.size();
        return area;
    default:
        auto checkSize = [&option](const Polygon& poly) {
            return poly.points.size() == static_cast<size_t>(option);
            };

        area = std::accumulate(
            figures.cbegin(),
            figures.cend(),
            0.0,
            [&checkSize](double area, const Polygon& poly) {
                return checkSize(poly) ? area + countShoelaceArea(poly) : area;
            }
        );
        return area;
    }
}

void maxAreaVertexes(const std::vector<Polygon>& figures, const int& option, std::ostream& out)
{
    StreamGuard stream(out);
    if (option == AREA)
    {
        auto comparator = std::bind(
            std::less<double>(),
            std::bind(countShoelaceArea, _1),
            std::bind(countShoelaceArea, _2)
        );
        auto max = std::max_element(figures.cbegin(), figures.cend(), comparator);
        out << std::fixed << std::setprecision(1) << ((max != figures.end()) ? countShoelaceArea(*max) : 0.0) << '\n';
    }
    else
    {
        auto comparator = std::bind(
            std::less<size_t>(),
            std::bind(
                &std::vector<Point>::size,
                std::bind(&Polygon::points, _1)
            ),
            std::bind(
                &std::vector<Point>::size,
                std::bind(&Polygon::points, _2)
            )
        );

        auto max = std::max_element(figures.cbegin(), figures.cend(), comparator);
        out << ((max != figures.end()) ? max->points.size() : 0.0) << '\n';
    }
}

void minAreaVertexes(const std::vector<Polygon>& figures, const int& option, std::ostream& out)
{
    StreamGuard stream(out);
    if (option == AREA)
    {
        auto comparator = std::bind(
            std::less<double>(),
            std::bind(countShoelaceArea, _1),
            std::bind(countShoelaceArea, _2)
        );
        auto min = std::min_element(figures.cbegin(), figures.cend(), comparator);
        out << std::fixed << std::setprecision(1) << ((min != figures.end()) ? countShoelaceArea(*min) : 0.0) << '\n';
    }
    else
    {
        auto comparator = std::bind(
            std::less<size_t>(),
            std::bind(
                &std::vector<Point>::size,
                std::bind(&Polygon::points, _1)
            ),
            std::bind(
                &std::vector<Point>::size,
                std::bind(&Polygon::points, _2)
            )
        );

        auto min = std::min_element(figures.cbegin(), figures.cend(), comparator);
        out << ((min != figures.end()) ? min->points.size() : 0.0) << '\n';
    }
}

int countFigures(const std::vector<Polygon>& figures, const int& option)
{
    int res = 0;
    if (option == EVEN)
    {
        res = std::count_if(figures.cbegin(), figures.cend(), [](const Polygon& poly) { return (poly.points.size() % 2 == 0); });
    }
    else if (option == ODD)
    {
        res = std::count_if(figures.cbegin(), figures.cend(), [](const Polygon& poly) { return (poly.points.size() % 2 == 1); });
    }
    else
    {
        res = std::count_if(figures.cbegin(), figures.cend(), [&](const Polygon& poly)
            {
                return (poly.points.size() == static_cast<size_t>(option));
            }
        );
    }
    return res;
}

int deleteFigures(std::vector<Polygon>& figures, const Polygon& targetPoly)
{
    int deletedCount = 0;
    bool previousMatch = false;

    auto newEnd = std::remove_if(figures.begin(), figures.end(),
        [&](const Polygon& poly) {
            if (poly == targetPoly) {
                if (previousMatch) {
                    ++deletedCount;
                    return true;
                }
                else {
                    previousMatch = true;
                    return false;
                }
            }
            else {
                previousMatch = false;
                return false;
            }
        });

    figures.erase(newEnd, figures.end());
    return deletedCount;
}

geometry::Point::Point(const Point& p)
{
    if (this != &p)
    {
        x = p.x;
        y = p.y;
    }
}

Point& geometry::Point::operator=(const Point& p)
{
    if (this != &p)
    {
        x = p.x;
        y = p.y;
    }
    return *this;
}

geometry::Point::Point(Point&& p) noexcept
{
    if (this != &p)
    {
        x = p.x;
        y = p.y;

        p.x = 0;
        p.y = 0;
    }
}


Point& geometry::Point::operator=(Point&& p) noexcept
{
    if (this != &p)
    {
        x = p.x;
        y = p.y;

        p.x = 0;
        p.y = 0;
    }
    return *this;
}


geometry::Polygon::Polygon(const Polygon& poly)
{
    points = poly.points;
}

Polygon& geometry::Polygon::operator=(const Polygon& poly)
{
    if (this != &poly)
    {
        points = poly.points;
    }
    return *this;
}

geometry::Polygon::Polygon(Polygon&& poly) noexcept : points(std::move(poly.points)) {};

Polygon& geometry::Polygon::operator=(Polygon&& poly) noexcept
{
    if (this != &poly)
    {
        points = std::move(poly.points);
    }
    return *this;
}


bool geometry::Polygon::operator==(const Polygon& otherPoly) const {
    if (points.size() != otherPoly.points.size())
    {
        return false;
    }
    return std::equal(points.begin(), points.end(), otherPoly.points.begin());
}

vecArr same::computeVectors(const std::vector<Point>& points, bool reverse = false)
{
    vecArr result;

    auto getVec = [&reverse](const Point& a, const Point& b)
        {
            if (reverse)
            {
                return std::make_pair((b.x - a.x), (b.y - a.y));
            }
            else
            {
                return std::make_pair((a.x - b.x), (a.y - b.y));
            }
        };

    std::transform(
        points.begin(), points.end() - 1,
        std::next(points.begin()),
        std::inserter(result, result.begin()),
        getVec
    );

    result.insert(getVec(points.back(), points.front()));

    return result;
}
