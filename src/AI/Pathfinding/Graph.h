#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "List.h"

class Graph{
public:
    /**
     * @brief Graph constructor
     * 
     */
    Graph();

    /**
     * @brief Graph destructor
     * 
     */
    ~Graph();

    /**
     * @brief Adds a connection between two nodes
     * 
     * @param float (connection cost)
     * @param Node (FromNode)
     * @param Node (ToNode)
     */
    void addConnection(float , Node*, Node*);

    /**
     * @brief Prints in terminal the actual graph connections
     * 
     */
    void printActualGraphConnections();

    /**
     * @brief Prints in terminal the list of connections from the node given
     * 
     * @param Node (Node to check)
     */
    void printNodeConnections(Node*);

        /**
     * @brief Returns the shortest path from the StartNode to the EndNode using Dijkstra Algorithm
     * 
     * @param Graph Graph
     * @param Node StartNode
     * @param Node EndNode
     * @return std::vector<Connection*> path
     */
    std::list<Connection*> *DijkstraPF(Node*, Node*);
    
private:
    std::vector<Connection*> m_connections;

    //Pathfinding variables
    List* m_openList;
    List* m_closedList;
    NodeRecord* m_startRecord;

};
#endif
