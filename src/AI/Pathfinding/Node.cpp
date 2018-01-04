#include "Node.h"

Node::Node(){
    m_ID = -1;
    m_RegionName = "NONE";
}

Node::Node(int id, std::string regionName){
    m_ID = id;
    m_RegionName = regionName;
}

Node::~Node(){
    for(int i = 0; i<m_connections.size(); i++) delete m_connections[i];
    m_connections.clear();
}

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
