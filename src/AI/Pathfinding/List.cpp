#include "List.h"

List::List(){
}

List::~List(){
    for(int i = 0; i<m_list.size();i++){
        delete m_list[i];
    }
    m_list.clear();
}

void List::add(NodeRecord* nr){
    int pos = getNodeRecordPosition(nr->m_node);
    if(pos != -1){
        std::cout<<"The node was in the list, update"<<std::endl;
        m_list[pos] = nr;
    }
    else{
        std::cout<<"the node was not in the list"<<std::endl;
        insertNode(nr);
    }
}

void List::remove(NodeRecord* nr) {
    float cost = nr->m_estimatedTotalCost;
    for(int i = 0; i<m_list.size(); i++){
        if(nr!= m_list[i]) m_list.erase(m_list.begin() + i);
        else if(cost<=m_list[i]->m_estimatedTotalCost){
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
    if(pos != -1 ) return m_list[pos];
    return NULL;
}

NodeRecord* List::smallestElement(){
    return getMin();
}

int List::size(){
    return m_list.size();
}

int List::getNodeRecordPosition(Node* n){
    for(int i = 0; i<m_list.size(); i++){
        if(m_list[i]->m_node == n){
            return i;
        }
    }
    return -1;
}

void List::printListOfNodes(){
    for(int i = 0; i < m_list.size(); i++){

        std::cout<<"---------- "<<i<<" ----------"<<std::endl;
        std::cout<<" Node ID: "<<m_list[i]->m_node->getNodeID()<<std::endl;
        std::cout<<" Node Region: "<<m_list[i]->m_node->getRegionName()<<std::endl;
        if(m_list[i]->m_connection!=NULL) std::cout<<" Connection from < : "<<m_list[i]->m_connection->getFromNode()->getRegionName()<<" // To > "<< m_list[i]->m_connection->getToNode()->getRegionName()<<std::endl;
        else std::cout<<" Connection is NULL"<<std::endl;
        std::cout<<" NodeRecord CostSoFar: "<<m_list[i]->m_costSoFar<<std::endl;
        std::cout<<" NodeRecord heuristic value: "<<m_list[i]->m_heuristic<<std::endl;
        std::cout<<" NodeRecord estimatedTotalCost: "<<m_list[i]->m_estimatedTotalCost<<"\n"<<std::endl;
    }
}

/**************************************************\ MIN HEAP FUNCTIONS \**************************************************/

void List::insertNode(NodeRecord* nr){
    //insert the new element at the end of the heap
    m_list.push_back(nr);
    
    //check if the heap property has been broken
    int i = m_list.size()-1;
    //while the parent node has bigger CostSoFar swap the nodes
    while (i != 0 && m_list[parentNodeIndex(i)]->m_estimatedTotalCost > m_list[i]->m_estimatedTotalCost)
    {
       swapNodes(m_list[i], m_list[parentNodeIndex(i)]);
       i = parentNodeIndex(i);
    }
}


NodeRecord* List::getMin(){
std::cout<<"*****************GETMIN()*****************"<<std::endl;
    printListOfNodes();
    //cojemos el primero del heap
    NodeRecord* min = m_list[0];
    std::cout<<"list size: "<<m_list.size()<<std::endl;
    //if the list has more than one element
    //if(m_list.size() != 1){
        //lo sustituimos por el ultimo y lo eliminamos del final
        std::cout<<"initial node before "<<m_list[0]->m_node->getRegionName()<<", "<<m_list[0]->m_estimatedTotalCost<<std::endl;
        m_list[0] = m_list[m_list.size()-1];
        std::cout<<"initial node now "<<m_list[0]->m_node->getRegionName()<<", "<<m_list[0]->m_estimatedTotalCost<<std::endl;
        m_list.erase(m_list.end()-1);
        std::cout<<"NEW list size: "<<m_list.size()<<std::endl;

        //ordenamos el heap desde el primer elemento
        heapify(0);
        printListOfNodes();
    //}
std::cout<<"*****************GETMIN RETURNS "<< min->m_node->getRegionName() <<"*****************"<<std::endl;
    return min;
}

void List::heapify(int root){
    //get the left node
    int l = leftNodeIndex(root);
    //get the right node
    int r = rightNodeIndex(root);
    //asume that the root is the smallest initially
    int min = root;
    //if left index is minor than m_list size and the cost so far of the left is minor that the root, the new smallest is the left
    if(l!= -1 && l<m_list.size() && m_list[min]->m_estimatedTotalCost > m_list[l]->m_estimatedTotalCost) min = l;
    //if left index is minor than m_list size and the cost so far of the right is minor that the root, the new smallest is the right
    if(r!=-1 && r<m_list.size() && m_list[min]->m_estimatedTotalCost > m_list[r]->m_estimatedTotalCost) min = r;

    //if the smallest is different from the root, switch the actual root and smallest and proceed to the next level of the heap
    if(min != root){
        swapNodes(m_list[root], m_list[min]);
        heapify(min);
    }
}

void List::swapNodes(NodeRecord* nr1, NodeRecord* nr2){

    NodeRecord aux = *nr1;

    *nr1 = *nr2;
    *nr2 = aux;
}

int List::parentNodeIndex(int index){
    int pos = (index-1)/ 2;
    if(pos >=0 && pos<m_list.size()) return pos;
    return -1;
}

int List::leftNodeIndex(int index){
    int pos = (2*index + 1);
    if(pos >=0 && pos<m_list.size()) return pos;
    return -1;
}

int List::rightNodeIndex(int index){
    int pos = (2*index + 2);
    if(pos >=0 && pos<m_list.size()) return pos;
    return -1;
}
