#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <iostream>
#include <vector>
#include <limits>

class List;
class Node;
class NavMesh;
class Heuristic;
class Connection;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Pathfinding{
    public:
    Pathfinding();
    ~Pathfinding();

    void ResetValues();
    bool AStar(vector3df from, vector3df to, vector3df firstC, vector3df secondC);

    int         GetIndexNearestNode(vector3df pos, int start=0);
    vector3df   GetPosNode(int index);

    private:
    Heuristic* heur;
    List* m_openList;
    List* m_closedList;

    std::vector<Connection*> m_connections;

    std::vector<Node*> m_path;  // Vector de nodos que marca el camino que ha de seguir la IA

    Node* StartNode;            // Nodo que se crea en la clase y que tenemos que reiniciar 
    Node* EndNode;              // Nodo que se crea en la clase y que tenemos que reiniciar

};

#endif