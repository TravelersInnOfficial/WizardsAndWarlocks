#ifndef NAVMESHLOADER_H
#define NAVMESHLOADER_H

#include <iostream>
#include "Objects/NavMesh.h"

class NavMeshLoader{
    public:
    NavMeshLoader();
    ~NavMeshLoader();
    void LoadNavMeshGraph(NavMesh* navmesh, std::string);

};

#endif