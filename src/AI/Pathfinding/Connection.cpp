#include "Connection.h"

Connection::Connection(){

}

Connection::~Connection(){

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