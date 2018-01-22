#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "List.h"
#include "Heuristic.h"
#include "./../../Objects/NavMesh.h"

class Pathfinding{
    public:
    Pathfinding();
    ~Pathfinding();

    void ResetValues();
    std::list<Connection*> AStar(vector3df , vector3df);

    int         GetIndexNearestNode(vector3df pos, int start=0);
    vector3df   GetPosNode(int index);

    private:
    Heuristic* heur;
    List* m_openList;
    List* m_closedList;
    //NodeRecord* m_startRecord;    
    Node* StartNode;    // Nodo que se crea en la clase y que tenemos que reiniciar 
    Node* EndNode;      // Nodo que se crea en la clase y que tenemos que reiniciar

    std::list<Connection*> *DijkstraPF(Node* StartNode, Node* GoalNode);

};

#endif