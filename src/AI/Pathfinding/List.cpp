#include "List.h"

#include "Node.h"
#include "Connection.h"
#include "NodeRecord.h"

List::List(){
}

List::~List(){
    clear();
}

void List::clear(){
    int size = m_list.size();
    for(int i = 0; i<size;i++){
        delete m_list[i];
    }
    m_list.clear();
}

void List::add(NodeRecord* nr){
    int pos = getNodeRecordPosition(nr->m_node);
    if(pos != -1){
        m_list[pos] = nr;
    }
    else{
        insertNode(nr);
    }
}

void List::remove(NodeRecord* nr) {
    int size = m_list.size();
    for(int i = 0; i<size; i++){
        NodeRecord* current = m_list[i]; 
        if(nr == current){
            m_list.erase(m_list.begin()+i);
            break;
        }
    }
}

bool List::contains(Node* n){
    int pos = getNodeRecordPosition(n);
    if(pos != -1) return true;
    return false;
}

NodeRecord* List::find(Node* n){
    int pos = getNodeRecordPosition(n);
    if(pos != -1 ){
        return m_list[pos];
    }
    return nullptr;
}

NodeRecord* List::smallestElement(){
    return getMin();
}

int List::size(){
    return m_list.size();
}

int List::getNodeRecordPosition(Node* n){
    int size = m_list.size();
    for(int i = 0; i<size; i++){
        if(m_list[i]->m_node == n){
            return i;
        }
    }
    return -1;
}

int List::getIndexNearestNode(vector3df pos, int start){
    int output = 0;
    int size = m_list.size();
    if(start < size){  
        vector3df nodePos = m_list[start]->m_node->getPosition();
        nodePos = nodePos - pos;
  
        float value = nodePos.length();
        output = start;
  
        for(int i=start; i<size; i++){
            nodePos = m_list[i]->m_node->getPosition();
            nodePos = nodePos - pos;
            if(nodePos.length() < value){
                value = nodePos.length();
                output = i;
            }
        }
    }
    return output;
}

vector3df List::getPosNode(int index){
    int size = m_list.size();
    if(index < size){
        return m_list[index]->m_node->getPosition();
    }
    vector3df output(0,0,0);
    return output;
}

void List::printListOfNodes(){
    int size = m_list.size();
    for(int i = 0; i < size; i++){

        std::cout<<"---------- "<<i<<" ----------"<<std::endl;
        std::cout<<" Node ID: "<<m_list[i]->m_node->getNodeID()<<std::endl;
        std::cout<<" Node Region: "<<m_list[i]->m_node->getRegionName()<<std::endl;
        if(m_list[i]->m_connection!=nullptr) std::cout<<" Connection from < : "<<m_list[i]->m_connection->getFromNode()->getRegionName()<<" // To > "<< m_list[i]->m_connection->getToNode()->getRegionName()<<std::endl;
        else std::cout<<" Connection is nullptr"<<std::endl;
        std::cout<<" NodeRecord CostSoFar: "<<m_list[i]->m_costSoFar<<std::endl;
        std::cout<<" NodeRecord heuristic value: "<<m_list[i]->m_heuristic<<std::endl;
        std::cout<<" NodeRecord estimatedTotalCost: "<<m_list[i]->m_estimatedTotalCost<<"\n"<<std::endl;
    }
}

/**************************************************\ MIN HEAP FUNCTIONS \**************************************************/

void List::insertNode(NodeRecord* nr){

    int size = m_list.size();
    for(int i=0; i<size; i++){
        if(nr->m_estimatedTotalCost < m_list[i]->m_estimatedTotalCost){
            m_list.insert(m_list.begin() + i, nr);
            return;
        }
    }

    m_list.push_back(nr);

}


NodeRecord* List::getMin(){
    NodeRecord* min = m_list[0];
    m_list.erase(m_list.begin());
    return min;
}
