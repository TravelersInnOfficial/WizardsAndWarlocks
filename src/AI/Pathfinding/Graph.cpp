#include "Graph.h"

Graph::Graph(){}
Graph::~Graph(){}

std::vector<Connection*> Graph::getNodeConnectionList(Node*){
    std::vector<Connection*> ret;

    return ret;
}

std::vector<Node*> Graph::getNodeList(){
    return m_nodeList;
}