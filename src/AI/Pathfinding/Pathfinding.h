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

    private:
    List* m_openList;
    List* m_closedList;
    NodeRecord* m_startRecord;
};

#endif