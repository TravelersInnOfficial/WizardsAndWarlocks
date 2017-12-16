#ifndef DIJKSTRAPATHFINDING_H
#define DIJKSTRAPATHFINDING_H

#include "Graph.h"

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
    std::list<Connection*> *makeDijkstraPahtfinding(Graph*, Node*, Node*);

private:

    List* m_openList;
    List* m_closedList;
    NodeRecord* m_startRecord;

};
#endif