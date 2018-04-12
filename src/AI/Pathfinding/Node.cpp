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

void Node::setData(int id, vector3df position){
    m_ID = id;
    m_position = position;
}

void Node::eraseConnection(Node* n){
    int size = m_connections.size();
    for(int i=size-1; i>=0; i--){
        Node* node = m_connections[i]->getToNode();
        if(node == n){
            m_connections.erase(m_connections.begin() + i);
            break;
        }
    }
}

Node::~Node(){
    int connectionSize = m_connections.size();
    for(int i = connectionSize -1 ; i>=0; i--) delete m_connections[i];
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