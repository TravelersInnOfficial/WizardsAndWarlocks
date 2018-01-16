#ifndef NAVMESHLOADER_H
#define NAVMESHLOADER_H

#include <iostream>
#include "AI/Pathfinding/Graph.h"
#include "AI/Pathfinding/NavMesh.h"

class NavMeshLoader{
    public:
    NavMeshLoader();
    ~NavMeshLoader();
    NavMesh LoadNavMeshGraph(std::string);

};

#endif