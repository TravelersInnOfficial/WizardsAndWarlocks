#include "Connection.h"

Connection::Connection(){
    m_cost = 0;
    m_FromNode  = NULL;
    m_ToNode = NULL;
}

Connection::Connection(float cost, Node* FromNode, Node* ToNode){
    m_cost = cost;
    m_FromNode = FromNode;
    m_ToNode = ToNode;
    m_FromNode->addConnection(this);
}

Connection::~Connection(){
    delete m_FromNode;
    delete m_ToNode;
}

float Connection::getCost(){
    return m_cost;
}

Node* Connection::getFromNode(){
    return m_FromNode;
}

Node* Connection::getToNode(){
    return m_ToNode;
}