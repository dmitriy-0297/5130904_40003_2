#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include "Point.h"
#include "Polygon.h"

using namespace std;


//конструкторы
Polygon::Polygon()
{
    vector<Point> points;
    points_ = points;
}

Polygon::Polygon(const vector<Point>& pts)
{
    points_ = pts;
}

Polygon::Polygon (string data)
{
    try
    {
        vector<Point> points;
        std::istringstream iss(data);
        string token;
        iss >> token;
        int vertexes = std::stoi(token);

        for (int i = 0; i < vertexes; i++)
        {
            if (!(iss >> token)) break;
            points.push_back(Point(token));
        }
        points_ = points;
    }
    catch (const InvalidPoint&)
    {
        throw InvalidPolygon();
    }
    catch (...)
    {
        throw InvalidPolygon();
    }
}


//операции
Polygon Polygon::operator+(const Polygon& other)
{
    vector<Point> combined = std::accumulate(
        points_.begin(),
        points_.end(),
        vector<Point>{},
        [](vector<Point> acc, const Point& p) {
            acc.push_back(p);
            return acc;
        }
        );

    combined = std::accumulate(
        other.points_.begin(),
        other.points_.end(),
        std::move(combined),
        [](vector<Point> acc, const Point& p)
        {
            acc.push_back(p);
            return acc;
        }
    );
    return Polygon(combined);
}

bool Polygon::operator==(const Polygon& other) const
{
    if (getVertexes() != other.getVertexes()) return false;
    return std::equal(
        points_.begin(), points_.end(),
        other.points_.begin(),
        [](const Point& a, const Point& b) {return a == b;}
    );
}

int Polygon::getVertexes() const
{
    return points_.size();
}

bool Polygon::isEven() const
{
    return (points_.size() % 2 == 0);
}

double Polygon::getArea() const
{
    double sum = std::inner_product(
        points_.begin(), points_.end()-1,
        std::next(points_.begin()),
        0.0,
        std::plus<>(),
        [](const Point& a, const Point& b) {return a.x_ * b.y_ - b.x_ * a.y_;}
    );
    sum += points_.back().x_ * points_.front().y_ - points_.front().x_ * points_.back().y_;

    return std::abs(sum)/2.0;
}

vector<Point> Polygon::sortPoints() const
{
    vector<Point> points = points_;
    std::sort(points.begin(), points.end());
    return points;
}

bool Polygon::isAlmostEqual(const Polygon& other) const
{
    if (getVertexes() != other.getVertexes()) return false;
    vector<Point> p1 = sortPoints();
    vector<Point> p2 = other.sortPoints();
    return std::equal(
        p1.begin(), p1.end(),
        p2.begin(),
        [](const Point& a, const Point& b) {return a == b;}
    );
}

//вспомогательные
Polygon Polygon::getSquare()
{
    Point yMax = *std::max_element(
        points_.begin(), points_.end(),
        [](const Point& p1, const Point& p2) {return p1.lessY(p2);}
    );
    Point yMin = *std::min_element(
        points_.begin(), points_.end(),
        [](const Point& p1, const Point& p2) {return p1.lessY(p2);}
    );
    Point xMax = *std::max_element(
        points_.begin(), points_.end(),
        [](const Point& p1, const Point& p2) {return p1.lessX(p2);}
    );
    Point xMin = *std::min_element(
        points_.begin(), points_.end(),
        [](const Point& p1, const Point& p2) {return p1.lessX(p2);}
    );
    vector<Point> squarePoints;
    squarePoints.push_back(Point(xMin.x_, yMax.y_));
    squarePoints.push_back(Point(xMax.x_, yMax.y_));
    squarePoints.push_back(Point(xMax.x_, yMin.y_));
    squarePoints.push_back(Point(xMin.x_, yMin.y_));

    return Polygon(squarePoints);
}

bool Polygon::fitsInFrame(const Point& p) const
{
    if (points_.size() != 4) return false;
    if (p.x_ < points_[0].x_) return false;
    if (p.x_ > points_[1].x_) return false;
    if (p.y_ < points_[3].y_) return false;
    if (p.y_ > points_[0].y_) return false;
    return true;
}

bool Polygon::isVertex(const Point& x)
{
    return !std::all_of(
        points_.begin(), points_.end(),
        [x](const Point& p){return !(x == p);}
    );
}


bool Polygon::isInside(const Point& p)
{
    if (isVertex(p)) return true;
    double totalAngle = std::inner_product(
        points_.begin(), points_.end() - 1,
        points_.begin() + 1,
        0.0,
        std::plus<>(),
        [&p](const Point& a, const Point& b) {return p.getAngle(a, b);}
    );

    totalAngle += p.getAngle(points_.back(), points_.front());
    return std::abs(totalAngle - 360.0) < 1e-2;
}

bool Polygon::isIntersecting(const Polygon& other)
{
    return !std::all_of(
        other.points_.begin(), other.points_.end(),
        [this](const Point& p) {return !isInside(p);}
    );
}

bool Polygon::isSame(const Polygon& other)
{
    if (points_.size() != other.getVertexes()) return false;
    vector<Point> p1 = sortPoints();
    vector<Point> p2 = other.sortPoints();
    Point firstDelta = p1[0].getDelta(p2[0]);
    return std::equal(
        p1.begin()+1, p1.end(),
        p2.begin()+1,
        [firstDelta](const Point& a, const Point& b) {return a.getDelta(b) == firstDelta;}
    );
}

bool Polygon::isRectangle()
{
    if (points_.size() != 4) return false;

    std::vector<int> counter(4);
    std::iota(counter.begin(), counter.end(), 0);

    int rightAngles = std::accumulate(
        counter.begin(), counter.end(),
        0,
        [this](int acc, int i)
        {
            const Point& a = points_[(i + 3) % 4];
            const Point& b = points_[i];
            const Point& c = points_[(i + 1) % 4];
            return acc + (b.isRight(a, c) ? 1 : 0);
        }
    );

    return (rightAngles == 4);
}

bool Polygon::hasRightAngles()
{
    int n = points_.size();
    std::vector<int> counter(n);
    std::iota(counter.begin(), counter.end(), 0);

    int rightAngles = std::accumulate(
        counter.begin(), counter.end(),
        0,
        [this, n](int acc, int i)
        {
            const Point& a = points_[(i + n - 1) % n];
            const Point& b = points_[i];
            const Point& c = points_[(i + 1) % n];
            return acc + (b.isRight(a, c) ? 1 : 0);
        }
    );

    return rightAngles > 0;
}

