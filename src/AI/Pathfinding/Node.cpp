#include "Node.h"

Node::Node(){}

Node::Node(int id, std::string regionName){
    m_ID = id;
    m_RegionName = regionName;
}

Node::~Node(){}

int Node::getNodeID(){
    return m_ID;
}

std::string Node::getRegionName(){
    return m_RegionName;
}

void Node::addConnection(Connection* c){
    m_connections.push_back(c);
}

std::vector<Connection*> Node::getOutgoingConnections(){
    return m_connections;
}
