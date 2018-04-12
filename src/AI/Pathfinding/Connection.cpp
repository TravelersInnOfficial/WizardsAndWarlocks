#include "Connection.h"

Connection::Connection(){
    m_cost = 0;
    m_FromNode  = nullptr;
    m_ToNode = nullptr;
}

Connection::Connection(float cost, Node* FromNode, Node* ToNode){
    m_cost = cost;
    m_FromNode = FromNode;
    m_ToNode = ToNode;
    m_FromNode->addConnection(this);
}

Connection::~Connection(){
    // Los nodos se eliminan en otras clases
}

void Connection::Unlink(){
    m_FromNode->eraseConnection(m_ToNode);
    m_ToNode->eraseConnection(m_FromNode);
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