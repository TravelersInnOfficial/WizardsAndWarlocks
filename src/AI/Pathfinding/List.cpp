#include "List.h"

List::List(){
    m_list = NULL;
}

List::~List(){}

NodeRecord* List::smallestElement(){
    NodeRecord* smallest = NULL;
    std::list<NodeRecord*>::iterator it = m_list->begin();
    for(; it!=m_list->end();++it){
        
    }

    return smallest;
}

int List::size(){
    return m_list->size();
}

bool List::contains(Node*){

    return false;
}

NodeRecord* List::find(Node*){
    NodeRecord* ret;

    return ret;
}

void List::add( NodeRecord* n) { 
    if(m_list->empty()){
        m_list->push_back(n);
    }
    else{
        std::list<NodeRecord*>::iterator it = m_list->begin();
        for(; it != m_list->end();++it){
           // if(n->m_costSoFar) m_list->emplace(it,n);
        }
    }
    
}

void List::remove(NodeRecord* n) { 
    m_list->remove(n);
}