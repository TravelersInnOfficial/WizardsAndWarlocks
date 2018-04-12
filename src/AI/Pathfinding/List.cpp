#include "List.h"

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
    /*
    float cost = nr->m_estimatedTotalCost;
    int size = m_list.size();
    for(int i = 0; i<size; i++){
        NodeRecord* current = m_list[i]; 
        if(nr!= current){ 
            m_list.erase(m_list.begin() + i);
        }
        else if(cost<=current->m_estimatedTotalCost){
            break;
        }
    }
    */

    float cost = nr->m_estimatedTotalCost;
    int size = m_list.size();
    for(int i = 0; i<size; i++){
        NodeRecord* current = m_list[i]; 
        if(nr == current){
            //m_list.erase(m_list.begin() + i);
            //cambiamos el seleccionado por el ultimo y hacemos un heapify
            m_list[i] = m_list[m_list.size()-1];
            
            delete m_list[size-1];
            m_list.erase(m_list.end()-1);

            heapify(i);
            break;
        }
        else if(cost<=current->m_estimatedTotalCost){
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
    //insert the new element at the end of the heap
    m_list.push_back(nr);
    
    //check if the heap property has been broken
    int i = m_list.size()-1;
    //while the parent node has bigger CostSoFar swap the nodes
    while (i != 0 && m_list[parentNodeIndex(i)]->m_estimatedTotalCost > m_list[i]->m_estimatedTotalCost)
    {
       swapNodes(i, parentNodeIndex(i));
       i = parentNodeIndex(i);
    }
}


NodeRecord* List::getMin(){
    NodeRecord* min = m_list[0];

    int size = m_list.size();
    m_list[0] = m_list[size-1];

    m_list.erase(m_list.end()-1);

    heapify(0);
    return min;
}

void List::heapify(int root){
    int listSize = m_list.size();
    //get the left node
    int l = leftNodeIndex(root);
    //get the right node
    int r = rightNodeIndex(root);
    //asume that the root is the smallest initially
    int min = root;
    //if left index is minor than m_list size and the cost so far of the left is minor that the root, the new smallest is the left
    if(l!= -1 && l<listSize && m_list[min]->m_estimatedTotalCost > m_list[l]->m_estimatedTotalCost) min = l;
    //if left index is minor than m_list size and the cost so far of the right is minor that the root, the new smallest is the right
    if(r!=-1 && r<listSize && m_list[min]->m_estimatedTotalCost > m_list[r]->m_estimatedTotalCost) min = r;

    //if the smallest is different from the root, switch the actual root and smallest and proceed to the next level of the heap
    if(min != root){
        swapNodes(root, min);
        heapify(min);
    }
}

void List::swapNodes(int nr1, int nr2){
    std::iter_swap(m_list.begin()+nr1, m_list.begin()+nr2);
    // Al crear un NodeRecord en el metodo al salir de este se elimina
    // Llamando a su destructor y eliminando cosas que no toca
    /*NodeRecord aux = *nr1;

    *nr1 = *nr2;
    *nr2 = aux;
    */
}

int List::parentNodeIndex(int index){
    int pos = (index-1)/ 2;
    int listSize = m_list.size();
    if(pos >=0 && pos<listSize) return pos;
    return -1;
}

int List::leftNodeIndex(int index){
    int pos = (2*index + 1);
    int listSize = m_list.size();
    if(pos >=0 && pos<listSize) return pos;
    return -1;
}

int List::rightNodeIndex(int index){
    int pos = (2*index + 2);
    int listSize = m_list.size();
    if(pos >=0 && pos<listSize) return pos;
    return -1;
}
