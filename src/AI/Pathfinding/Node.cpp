#include "Node.h"

Node::Node(){}

Node::Node(int id){
    m_ID = id;
}

Node::~Node(){}

int Node::getNodeID(){
    return m_ID;
}