#ifndef NAVMESH_H
#define NAVMESH_H

#include "./../AI/Pathfinding/Node.h"
#include "./../AI/Pathfinding/Connection.h"

struct Triangle{
    std::vector<Node*> vertices;
};

class NavMesh{
    public:
    NavMesh();
    NavMesh(std::vector<Node*>, std::vector<Connection*>, std::vector<Triangle*>);
    ~NavMesh();
    void addNode(int id, vector3df position);
    void addConnection(float cost, int from_index, int to_index);
    void addTriangle(int vertex1, int vertex2, int vertex3);
    std::vector<Node*> *searchNearestNodes(vector3df);
    std::vector<Triangle*> getTriangles();
    void printData();

    private:
    std::vector<Triangle*> m_triangles;
    std::vector<Node*> m_nodes;
    std::vector<Connection*> m_connections;

    bool pointInTriangle(vector3df, std::vector<vector3df>);
    float dotProduct(vector3df, vector3df);
    vector3df crossProduct(vector3df, vector3df);
    bool sameSide(vector3df, vector3df, vector3df, vector3df);

};

#endif