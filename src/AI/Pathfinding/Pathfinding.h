#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Graph.h"
#include "List.h"
#include "NavMesh.h"

class Pathfinding{
    public:
    Pathfinding();
    ~Pathfinding();
    std::list<Connection*> *makeAStar(NavMesh, vector3df, vector3df);

    private:
    List* m_openList;
    List* m_closedList;
    NodeRecord* m_startRecord;    

    std::list<Connection*> *DijkstraPF(Graph* g, Node* StartNode, Node* GoalNode);
    std::list<Connection*> *AStar(Node* StartNode , Node* EndNode);

};

#endif