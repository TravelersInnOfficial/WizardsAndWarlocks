#include "Graph.h"

Graph::Graph(){}
Graph::~Graph(){}

std::vector<Connection*> Graph::getNodeConnectionList(Node*){
    std::vector<Connection*> ret;

    return ret;
}

List* Graph::getNodeList(){
    return m_nodeList;
}