#ifndef COMMANDS_H
#define COMMANDS_H

#include "Polygons.h"

namespace kirillova
{
    void executeCommands(const std::vector<Polygon>& polygons,
        const std::string& command,
        const std::string& arguments);
}

#endif
