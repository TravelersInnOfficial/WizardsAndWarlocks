#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "List.h"
#include "./../../Objects/NavMesh.h"

class Pathfinding{
    public:
    Pathfinding();
    ~Pathfinding();
    std::list<Connection*> *AStar(vector3df , vector3df);

    private:
    List* m_openList;
    List* m_closedList;
    NodeRecord* m_startRecord;    
    Node* StartNode;
    Node* EndNode;

    std::list<Connection*> *DijkstraPF(Node* StartNode, Node* GoalNode);

};

#endif