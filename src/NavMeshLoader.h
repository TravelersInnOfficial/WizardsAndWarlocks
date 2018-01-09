#ifndef NAVMESHLOADER_H
#define NAVMESHLOADER_H

#include <iostream>
#include "AI/Pathfinding/Graph.h"

class NavMeshLoader{
    public:
    NavMeshLoader();
    ~NavMeshLoader();
    Graph LoadNavMeshGraph(std::string);

};

#endif