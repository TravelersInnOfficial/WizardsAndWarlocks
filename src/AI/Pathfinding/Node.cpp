#include "Node.h"
#include "Connection.h"

Node::Node(){
    m_ID = -1;
    m_RegionName = "NONE";
    m_position = vector3df(0,0,0);
}

Node::Node(int id, vector3df position){
    m_ID = id;
    m_position = position;
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

vector3df Node::getPosition(){
    return m_position;
}