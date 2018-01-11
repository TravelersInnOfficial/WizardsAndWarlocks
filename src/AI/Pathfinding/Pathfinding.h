#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Graph.h"
#include "List.h"

class Pathfinding{
    public:
    Pathfinding();
    ~Pathfinding();
    std::list<Connection*> *DijkstraPF(Graph* g, Node* StartNode, Node* GoalNode);
    std::list<Connection*> *AStar(Graph* g, Node* StartNode , Node* EndNode);
    bool pointInTriangle(vector3df, std::vector<vector3df>);

    private:
    List* m_openList;
    List* m_closedList;
    NodeRecord* m_startRecord;    
    float dotProduct(vector3df, vector3df);
    vector3df crossProduct(vector3df, vector3df);
    bool sameSide(vector3df, vector3df, vector3df, vector3df);

};

#endif