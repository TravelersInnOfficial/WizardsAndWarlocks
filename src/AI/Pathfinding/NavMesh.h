#ifndef NAVMESH_H
#define NAVMESH_H

#include "Graph.h"
struct Triangle{
    std::vector<Node*> vertices;
};

class NavMesh{
    public:
    NavMesh();
    NavMesh(std::vector<Node*>, std::vector<Connection*>, std::vector<Triangle*>);
    ~NavMesh();
    std::vector<Node*> *searchNearestNodes(vector3df);
    std::vector<Triangle*> getTriangles();
    Graph getGraph();
    void printData();

    private:
    std::vector<Triangle*> m_triangles;
    std::vector<Node*> m_nodes;
    std::vector<Connection*> m_connections;
    Graph* m_graph;

    bool pointInTriangle(vector3df, std::vector<vector3df>);
    float dotProduct(vector3df, vector3df);
    vector3df crossProduct(vector3df, vector3df);
    bool sameSide(vector3df, vector3df, vector3df, vector3df);

};

#endif