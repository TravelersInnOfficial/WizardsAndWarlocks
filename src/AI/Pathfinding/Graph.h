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
     * @brief Returns an array of connections outgoing from the given node
     * 
     * @param Node givenNode
     * @return std::vector<Connection*> nodeConnections
     */
    std::vector<Connection*> getNodeConnectionList(Node*);

    /**
     * @brief Returns a List with all graph nodeRecords
     * 
     * @return List* (All NodeRecords)
     */
    List* getNodeList();
    
private:
    List* m_nodeList;

};
#endif
