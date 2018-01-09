#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Node.h"
#include "Connection.h"

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
     * @brief Adds a new connection between two nodes inside the graph
     * 
     * @param float (cost)
     * @param int (from node index)
     * @param int (to node index)
     */
    void addConnection(float ,int, int);

    /**
     * @brief Adds a new node to the graph
     * 
     * @param int (ID)
     * @param vector3df (Position)
     */
    void addNode(int, vector3df);

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
     * @brief Returns all the connections of the graphs
     * 
     * @return std::vector<Connection*> (AllConnections)
     */
    std::vector<Connection*> getConnections();

    /**
     * @brief Returns all the nodes of the graph
     * 
     * @return std::vector<Node*> (AllNodes)
     */
    std::vector<Node*> getNodes();

    
private:
    std::vector<Connection*> m_connections;
    std::vector<Node*> m_nodes;
};
#endif
