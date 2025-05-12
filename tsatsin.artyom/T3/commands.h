#ifndef COMMANDS_H
#define COMMANDS_H

#include "geometry.h"
#include <functional>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace artemonts
{
    std::map<std::string, std::function<void(std::istream&, std::ostream&)>>
        makeCommands(const std::vector<Polygon>& polys);
}

#endif
