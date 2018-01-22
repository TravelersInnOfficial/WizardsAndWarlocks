#ifndef NODERECORD_H
#define NODERECORD_H

#include "Node.h"
#include "Connection.h"

//This structure is used to keep track of the information we need for each node
struct NodeRecord{
public:
	NodeRecord(){
		m_node = NULL;
		m_connection = NULL;
		m_costSoFar = 0;
   		m_estimatedTotalCost = 0;
    	m_heuristic = 0;
	}
	~NodeRecord(){
		// Los nodos se eliminan con el navmesh
		// Las conexiones se eliminan en otro sitio
	}
    Node* m_node;
    Connection *m_connection;
    float m_costSoFar;
    float m_estimatedTotalCost;
    float m_heuristic;
};

#endif