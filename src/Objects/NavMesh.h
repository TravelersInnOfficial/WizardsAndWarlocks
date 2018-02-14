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
    void Draw();
    void AddNode(int id, vector3df position);
    void AddConnection(float cost, int from_index, int to_index);
    void AddTriangle(int vertex1, int vertex2, int vertex3);
    std::vector<Node*> SearchNearestNodes(vector3df);
    std::vector<Node*> SearchNearestNodes(vector3df pos, vector3df firstC, vector3df secondC);

    std::vector<Triangle*> GetTriangles();
    std::vector<Node*> GetNodes();
    std::vector<Connection*> GetConnections();
    void PrintData();

private:
    bool CheckInside(float A, float B, float C);

    std::vector<Triangle*> m_triangles;
    std::vector<Node*> m_nodes;
    std::vector<Connection*> m_connections;

};

#endif