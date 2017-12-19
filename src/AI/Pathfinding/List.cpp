#include "List.h"

List::List(){
    m_list = new std::list<NodeRecord*>();
}

List::~List(){}

NodeRecord* List::smallestElement(NodeRecord* n){
    //if theres no connections in the list return the original node
    NodeRecord* nextNode = NULL;
    std::vector<Connection*> c = n->m_node->getOutgoingConnections();
    std::cout<<c.size()<<std::endl;
    float smallestCost = 99999;
    for(int i = 0; i<c.size();i++){
        if(c[i]->getCost() < smallestCost){
            smallestCost = c[i]->getCost();
            nextNode = find(c[i]->getToNode());
            if(nextNode != NULL) std::cout<<"smallestElement - NextNode "<<nextNode->m_node->getRegionName()<<" with cost: "<<smallestCost<<std::endl;
            else std::cout<<"smallestElement - NextNode is null"<<std::endl;
        }
    }
    if(nextNode == NULL) return n;
    return nextNode;
}

int List::size(){
    return m_list->size();
}

bool List::contains(Node* n){
    //std::cout<<"Does the list contains :: "<<n->getRegionName()<<std::endl;
    NodeRecord* ret = new NodeRecord();
    std::list<NodeRecord*>::iterator it = m_list->begin();
    for(; it != m_list->end(); ++it){
        ret = *it;
        //std::cout<<ret->m_node->getRegionName()<<" ?= "<<n->getRegionName()<<std::endl;
        if(ret->m_node == n){ return true;}
    }
    return false;
}

NodeRecord* List::find(Node* n){
    std::cout<<"finding node..."<<std::endl;
    NodeRecord* ret = new NodeRecord();
    std::list<NodeRecord*>::iterator it = m_list->begin();
    for(; it != m_list->end(); ++it){
        ret = *it;
        if(ret->m_node == n){ std::cout<<"found node in list"<<std::endl;return ret;}
    }
    std::cout<<"NO MATCHES IN THE LIST"<<std::endl;
    return NULL;
}

void List::add(NodeRecord* n){
    if(contains(n->m_node)){
        std::cout<<"node was already in the list, update"<<std::endl;
        NodeRecord* nr = find(n->m_node);
        nr = n;
    }
    else{
        std::cout<<"the node was not in the list"<<std::endl;
        m_list->push_back(n);
    }
}

void List::remove(NodeRecord* n) { 
    NodeRecord* nr = find(n->m_node); 
    if(nr!=NULL) m_list->remove(nr);
}

void List::printListOfNodes(){
    std::list<NodeRecord*>::iterator it = m_list->begin();
    int i = 0;
    for(; it != m_list->end(); ++it){
        NodeRecord* nr = *it;
        std::cout<<"---------- "<<i<<" ----------"<<std::endl;
        std::cout<<" Node ID: "<<nr->m_node->getNodeID()<<std::endl;
        std::cout<<" Node Region: "<<nr->m_node->getRegionName()<<std::endl;
        if(nr->m_connection!=NULL) std::cout<<" Connection from < : "<<nr->m_connection->getFromNode()->getRegionName()<<" // To > "<< nr->m_connection->getToNode()->getRegionName()<<std::endl;
        else std::cout<<" Connection is NULL"<<std::endl;
        std::cout<<" NodeRecord CostSoFar: "<<nr->m_costSoFar<<"\n"<<std::endl;
        i++;
    }
}
