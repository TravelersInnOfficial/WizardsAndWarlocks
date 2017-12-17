#include "List.h"

List::List(){
    m_list = new std::list<NodeRecord*>;
}

List::~List(){}

NodeRecord* List::smallestElement(Node* n){
    std::vector<Connection*> c = n->getOutgoingConnections();
    float smallestCost = 99999;
    NodeRecord* nextNode = new NodeRecord();
    for(int i = 0; i<c.size();i++){
        if(c[i]->getCost() < smallestCost){ 
            smallestCost = c[i]->getCost();
            nextNode->m_node = c[i]->getToNode();
            nextNode->m_connection = c[i];
            //nextNode->m_costSoFar = c[i]->getCost();
        }
    }
    return nextNode;
}

int List::size(){
    return m_list->size();
}

bool List::contains(Node* n){
    NodeRecord* ret;
    std::list<NodeRecord*>::iterator it = m_list->begin();
    for(; it != m_list->end(); ++it){
        ret = *it;
        if(ret->m_node == n){ return true;}
    }
    return false;
}

NodeRecord* List::find(Node* n){
    NodeRecord* ret;
    std::list<NodeRecord*>::iterator it = m_list->begin();
    for(; it != m_list->end(); ++it){
        ret = *it;
        if(ret->m_node == n){ return ret;}
    }
    return NULL;
}

void List::add(NodeRecord* n){
    m_list->push_back(n);
}

void List::remove(NodeRecord* n) { 
    m_list->remove(n);
}

std::vector<Connection*>* List::getNodeConnections(Node* n){
    std::vector<Connection*> *list = NULL;

    std::list<NodeRecord*>::iterator it = m_list->begin();
    for(; it!=m_list->end(); ++it){
        NodeRecord *node = *it;
        if(node->m_node == n) list->push_back(node->m_connection);
    }
    return list;
}