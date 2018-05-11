#ifndef NAVMESHLOADER_H
#define NAVMESHLOADER_H

#include <iostream>

class NavMesh;

class NavMeshLoader{
    public:
    NavMeshLoader();
    ~NavMeshLoader();
    static void LoadNavMeshGraph(NavMesh* navmesh, std::string);

};

#endif