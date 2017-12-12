#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"
#include "Connection.h"
#include <vector>

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
     * @brief Returns an array of connections outgoing from the given node
     * 
     * @param Node givenNode
     * @return std::vector<Connection*> nodeConnections
     */
    std::vector<Connection*> getNodeConnectionList(Node*);

    /**
     * @brief Returns a list with all graph nodes
     * 
     * @return std::vector<Node*> allNodes
     */
    std::vector<Node*> getNodeList();
    
private:
    std::vector<Node*> m_nodeList;

};
#endif
