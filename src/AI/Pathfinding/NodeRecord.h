#ifndef NODERECORD_H
#define NODERECORD_H

#include "Node.h"
#include "Connection.h"

//This structure is used to keep track of the information we need for each node
struct NodeRecord{
public:

    Node* m_node;
    Connection *m_connection;
    float m_costSoFar;
    float m_estimatedTotalCost;
    float m_heuristic;
};

#endif