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
#include "PolygonHolder.h"

using namespace std;

//конструкторы
PolygonHolder::PolygonHolder(const std::vector<Polygon>& ps)
{
    polygons_ = ps;
}

PolygonHolder::PolygonHolder()
{
    vector<Polygon> ps;
    polygons_ = ps;
}

//вспомогательные
Polygon PolygonHolder::getFrame()
{
    Polygon allPoints = std::accumulate(
        polygons_.begin(), polygons_.end(),
        Polygon(),
        [](Polygon acc, const Polygon& p) {return (acc + p);}
    );
    return allPoints.getSquare();
}

void PolygonHolder::add(const Polygon& p)
{
    polygons_.push_back(p);
}

//основные

//площади
double PolygonHolder::getAREA(bool even)
{
    if (polygons_.size() == 0) return 0;

    return std::accumulate(
        polygons_.begin(),
        polygons_.end(),
        0.0,
        [even](double acc, Polygon& p) {return (p.isEven() == even) ? acc + p.getArea() : acc;}
    );
}

double PolygonHolder::getAREA()
{
    if (polygons_.size() == 0) return 0;

    double area =  std::accumulate(
        polygons_.begin(),
        polygons_.end(),
        0.0,
        [](double acc, Polygon p) {return acc + p.getArea();}
    );
    return area / polygons_.size();
}

double PolygonHolder::getAREA(int vertexes)
{
    if (polygons_.size() == 0) return 0;

    return std::accumulate(
        polygons_.begin(),
        polygons_.end(),
        0.0,
        [vertexes](double acc, Polygon p) {return (p.getVertexes() == vertexes) ? acc + p.getArea() : acc;}
    );
}


//максимумы и минимумы
double PolygonHolder::getMAX(bool area)
{
    if (area)
    {
        Polygon pmax = *std::max_element(
            polygons_.begin(), polygons_.end(),
            [](const Polygon& p1, const Polygon& p2) {return p1.getArea() < p2.getArea();}
        );
        return pmax.getArea();
    }
    else
    {
        Polygon pmax = *std::max_element(
            polygons_.begin(), polygons_.end(),
            [](const Polygon& p1, const Polygon& p2) {return p1.getVertexes() < p2.getVertexes();}
        );
        return double(pmax.getVertexes());
    }
}

double PolygonHolder::getMIN(bool area)
{
    if (area)
    {
        Polygon pmin = *std::min_element(
            polygons_.begin(), polygons_.end(),
            [](const Polygon& p1, const Polygon& p2) {return p1.getArea() < p2.getArea();}
        );
        return pmin.getArea();
    }
    else
    {
        Polygon pmin = *std::min_element(
            polygons_.begin(), polygons_.end(),
            [](const Polygon& p1, const Polygon& p2) {return p1.getVertexes() < p2.getVertexes();}
        );
        return pmin.getVertexes();
    }
}


//подсчет фигур
int PolygonHolder::getCOUNT(bool even)
{
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [even](Polygon& p) {return (p.isEven() == even);}
    );
}

int PolygonHolder::getCOUNT(int vertexes)
{
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [vertexes](Polygon& p) {return p.getVertexes() == vertexes;}
    );
}

//подсчет перестановок
int PolygonHolder::getPERMS(string data)
{
    Polygon other(data);
    int vertexes = other.getVertexes();
    return std::accumulate(
        polygons_.begin(), polygons_.end(),
        0,
        [vertexes, &other](int acc, const Polygon& p) {return acc + ((p.getVertexes() == vertexes && p.isAlmostEqual(other)) ? 1 : 0);}
    );
}

//подсчет идущих подряд фигур
int PolygonHolder::getMAXSEQ(string data)
{
    Polygon other(data);
    return std::inner_product(
        polygons_.begin(), polygons_.end() - 1,
        polygons_.begin() + 1,
        0,
        std::plus<>(),
        [&other](const Polygon& p1, const Polygon& p2) {return (p1 == other && p1 == p2) ? 1 : 0;}
    ) + 1;
}

//удаление дубликатов
int PolygonHolder::getRMECHO(string data)
{
    Polygon other(data);
    int removed = 0;
    vector<Polygon> cleaned;
    std::unique_copy(
        polygons_.begin(), polygons_.end(),
        std::back_inserter(cleaned),
        [&other](const Polygon& a, const Polygon& b) {return (a == b && a == other);}
    );
    removed = polygons_.size() - cleaned.size();
    polygons_ = cleaned;
    return removed;
}

//дублирование
int PolygonHolder::getECHO(string data)
{
    Polygon other(data);
    vector<Polygon> duplicated = std::accumulate(
        polygons_.begin(), polygons_.end(),
        vector<Polygon>{},
        [&other](std::vector<Polygon> acc, const Polygon& p) {
            acc.push_back(p);
            if (p == other) acc.push_back(p);
            return acc;
        }
    );
    int added = duplicated.size() - polygons_.size();
    polygons_ = duplicated;
    return added;
}

//меньшая площадь
int PolygonHolder::getLESSAREA(string data)
{
    Polygon other(data);
    double oarea = other.getArea();
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [oarea](Polygon& p) {return p.getArea() < oarea;}
    );
}

//в прямоугольнике ограничивающем все фигуры
string PolygonHolder::getINFRAME(string data)
{
    Polygon other(data);
    Polygon frame = getFrame();
    bool fits = std::all_of(
        other.points_.begin(), other.points_.end(),
        [frame](const Point& p) {return frame.fitsInFrame(p);}
    );
    if (fits) return "<TRUE>";
    else return "<FALSE>";
}

//пересечения
int PolygonHolder::getINTERSECTIONS(string data)
{
    Polygon other(data);
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [other](Polygon& p) {return p.isIntersecting(other);}
    );
}

//совместимость наложением
int PolygonHolder::getSAME(string data)
{
    Polygon other(data);
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [other](Polygon& p){return p.isSame(other);}
    );
}

//подсчет прямоугольников
int PolygonHolder::getRECTS()
{
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [](Polygon& p){return p.isRectangle();}
    );
}

//подсчет фигур с прямыми углами
int PolygonHolder::getRIGHTSHAPES()
{
    return std::count_if(
        polygons_.begin(), polygons_.end(),
        [](Polygon& p){return p.hasRightAngles();}
    );
}
