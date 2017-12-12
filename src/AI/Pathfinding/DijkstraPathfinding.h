#ifndef DIJKSTRAPATHFINDING_H
#define DIJKSTRAPATHFINDING_H

#include "Graph.h"

#include <vector>
//This structure is used to keep track of the information we need for each node
struct NodeRecord{
    Node* m_node;
    Connection* m_connection;
    float m_costSoFar;
};

class DijkstraPahtfinding{
public:
    /**
     * @brief Returns the shortest path from the StartNode to the EndNode
     * 
     * @param Graph Graph
     * @param Node StartNode
     * @param Node EndNode
     * @return std::vector<Connection*> path
     */
    std::vector<Connection*> makeDijkstraPahtfinding(Graph*, Node*, Node*);
    
    /**
     * @brief Returns the smalles element from the open list
     * 
     * @return Node* smallestNode
     */
    Node* getOpenListSmallestElement();

private:
    std::vector<Connection*> m_path;
    std::vector<Node*> m_openList;
    std::vector<Node*> m_closedList;
    NodeRecord* m_startRecord;

};
#endif