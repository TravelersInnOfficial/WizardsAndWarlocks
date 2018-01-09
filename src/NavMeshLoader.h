#ifndef NAVMESHLOADER_H
#define NAVMESHLOADER_H

#include <iostream>
#include "AI/Pathfinding/Graph.h"

class NavMeshLoader{
    public:
    NavMeshLoader();
    ~NavMeshLoader();
    bool LoadNavMesh(std::string jsonPath);
    std::vector<Node*> getNavMeshNodes();
    std::vector<Connection*> getNavMeshConnections();
    Graph getNavMeshGraph();

    private:
    std::vector<Node*> nodesArray;
    Graph graph;

};

#endif