#include "Graph.h"

Graph::Graph(){}
Graph::~Graph(){}

void Graph::addConnection(float cost, Node* FromNode, Node* ToNode){
    Connection* c = new Connection(cost, FromNode, ToNode);
    m_connections.push_back(c);
}

void Graph::printActualGraphConnections(){
    std::cout<<"\n----Actual graph connections----"<<std::endl;
    for(int i = 0; i<m_connections.size();i++){
        std::string fromroom = m_connections[i]->getFromNode()->getRegionName();
        std::string toroom = m_connections[i]->getToNode()->getRegionName();
        std::cout<<"Connection "<<i<<" : from "<<fromroom<<" to "<<toroom<<std::endl;
    }
}

void Graph::printNodeConnections(Node* n){
    std::cout<<"\n----Node connections----"<<std::endl;
    std::cout<<"Node ID: "<<n->getNodeID()<<" / Node Region: "<<n->getRegionName()<<std::endl;
    std::vector<Connection*> c = n->getOutgoingConnections();
    for(int i = 0; i<c.size(); i++){
        std::cout<<"Connection "<< i <<": "<<c[i]->getFromNode()->getRegionName()<<" > "<<c[i]->getToNode()->getRegionName()<<std::endl;
    }
}

std::list<Connection*> *Graph::DijkstraPF(Node* StartNode, Node* GoalNode){
    //Initialize the record for the start node
    m_startRecord = new NodeRecord();
    m_startRecord->m_node = StartNode;
    m_startRecord->m_connection = NULL;
    m_startRecord->m_costSoFar = 0;

    //Initialize the open and closed lists
    m_openList = new List();
    m_openList->add(m_startRecord);
    m_closedList = new List();
    
    //Iterate through processing each node
    NodeRecord* current = m_startRecord;
    Node* endNode = new Node();
    float endNodeCost = 0;
    NodeRecord* endNodeRecord = new NodeRecord();
    std::vector<Connection*> nodeConnections;
    std::list<Connection*> *path = new std::list<Connection*>();

    while(m_openList->size()>0){
        //Compile the list of connections in the path
        if(current != m_startRecord) path->push_back(current->m_connection);

        //Find the smallest element in the open list
        current = m_openList->smallestElement(current->m_node);

        //If it is the goal node, then terminate
        if(current->m_node == GoalNode){     
            path->push_back(current->m_connection);
            break;
        }

        //Otherwise get its outgoing connections
        nodeConnections = current->m_node->getOutgoingConnections();

        //Loop through each connection in turn
        for(int i = 0; i< nodeConnections.size();i++){
            //Get the cost estimate for the end node 
            endNode = nodeConnections.at(i)->getToNode();
            endNodeCost =  current->m_costSoFar + nodeConnections.at(i)->getCost();

            //Skip if the node is closed
            if(m_closedList->contains(endNode)) continue;
                
            //.. or if it is open and we’ve found a worse route
            else if(m_openList->contains(endNode)){
                //Here we find the record in the open list corresponding to the endNode.
                endNodeRecord = m_openList->find(endNode);
                if(endNodeRecord!=NULL && endNodeRecord->m_costSoFar <= endNodeCost) continue;
            }   
            
            //Otherwise we know we’ve got an unvisited node, so make a record for it
            else{
                //NodeRecord* endNodeRecord = new NodeRecord();
                endNodeRecord->m_node = endNode;
                
                //We’re here if we need to update the node. Update the cost and connection
                endNodeRecord->m_costSoFar = endNodeCost;
                endNodeRecord->m_connection = nodeConnections.at(i);
                
                //And add it to the open list
                if(!m_openList->contains(endNode)) m_openList->add(endNodeRecord);
            }
        }        
        //We’ve finished looking at the connections for the current node, so add it to the closed list 
        //and remove it from the open list
        m_openList->remove(current);
        m_closedList->add(current);
    }

    //We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
    if(current->m_node != GoalNode){
        //We’ve run out of nodes without finding the goal, so there’s no solution
        return NULL;
    }
    else{
        return path;
    }
}