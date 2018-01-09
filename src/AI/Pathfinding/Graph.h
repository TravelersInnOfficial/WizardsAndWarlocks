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
     * @brief Returns all the connections of the graphs
     * 
     * @return std::vector<Connection*> (AllConnections)
     */
    std::vector<Connection*> getConnections();

    
private:
    std::vector<Connection*> m_connections;
};
#endif
