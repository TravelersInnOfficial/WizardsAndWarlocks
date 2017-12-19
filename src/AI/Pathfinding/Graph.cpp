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
    std::cout<<"Node ID: "<<n->getNodeID()<<" / Node Region: "<<n->getRegionName()<<" :: \n"<<std::endl;
    std::vector<Connection*> c = n->getOutgoingConnections();
    for(int i = 0; i<c.size(); i++){
        std::cout<<"Connection "<< i <<": "<<c[i]->getFromNode()->getRegionName()<<" > "<<c[i]->getToNode()->getRegionName()<<std::endl;
    }
}

std::list<Connection*> *Graph::DijkstraPF(Node* StartNode, Node* GoalNode){
    std::cout<<"---------------Starting Dijkstra PF---------------"<<std::endl;
    //Initialize the record for the start node
    m_startRecord = new NodeRecord();
    m_startRecord->m_node = StartNode;
    m_startRecord->m_connection = NULL;
    m_startRecord->m_costSoFar = 0;
    m_startRecord->previous = 0;

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
        std::cout<<"OPEN LIST SIZE::: "<<m_openList->size()<<std::endl;
        std::cout<<"\n--------------------------------------------------------------------"<<std::endl;
        std::cout<<"--------------------INITIAL OPEN LIST--------------------"<<std::endl;
        m_openList->printListOfNodes();
        std::cout<<"--------------------INITIAL CLOSED LIST--------------------"<<std::endl;
        m_closedList->printListOfNodes();
        std::cout<<"--------------------------------------------------------------------\n"<<std::endl;

        //Find the smallest element in the open list
        current = m_openList->smallestElement(current);

        if(current->previous!=0)std::cout<<"previous node:" << current->previous->m_node->getRegionName()<<std::endl;
        std::cout<<"--------------------------- CURRENT NODE "<<current->m_node->getRegionName()<<" ---------------------------"<<std::endl;
        
         //If it is the goal node, then terminate
        if(current->m_node == GoalNode){ 
            std::cout<<"!! FOUND THE GOAL NODE "<<std::endl;
            break;
        }

        //Otherwise get its outgoing connections
        nodeConnections = current->m_node->getOutgoingConnections();

        //Loop through each connection in turn
        std::cout<<"NODE CONNECTIONS SIZE: "<<nodeConnections.size()<<std::endl;
        if(nodeConnections.size()==0) break;
        
        for(int i = 0; i < nodeConnections.size();i++){
            std::cout<<"checking connection "<<i<<std::endl;
            //Get the cost estimate for the end node 
            endNode = nodeConnections.at(i)->getToNode();
            
            std::cout<<"This connection leads to: "<<endNode->getRegionName()<<std::endl;
            endNodeCost =  current->m_costSoFar + nodeConnections.at(i)->getCost();

            //Skip if the node is closed
            if(m_closedList->contains(endNode)){ 
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the CLOSED list\n"<<std::endl;
                continue;
            }
             //.. or if it is open and we’ve found a worse route
            else if(m_openList->contains(endNode)){
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the OPEN list\n"<<std::endl;
                //Here we find the record in the open list corresponding to the endNode.
                endNodeRecord = m_openList->find(endNode);
                if(endNodeRecord!=NULL && endNodeRecord->m_costSoFar <= endNodeCost){ 
                    std::cout<<" CURRENT COSTSOFAR "<< endNodeCost<< " // COSTSOFAR FOUND "<< endNodeRecord->m_costSoFar<<std::endl;
                    continue;
                }
            } 
            else{
                std::cout<<"We have an unvisited node "<<endNode->getRegionName()<<std::endl;
                //Otherwise we know we’ve got an unvisited node, so make a record for it
                endNodeRecord = new NodeRecord();
                endNodeRecord->m_node = endNode;
            }

            //We’re here if we need to update the node. Update the cost and connection
            std::cout<<"----EndNodecost: "<<endNodeCost<<"----"<<std::endl;
            endNodeRecord->m_costSoFar = endNodeCost;
            endNodeRecord->m_connection = nodeConnections.at(i);
            endNodeRecord->previous = current;

            //And add it to the open list
            if(!m_openList->contains(endNode)){
                std::cout<<"The open list doesnt contain the new node:: "<<endNode->getRegionName()<<" Cost:" <<endNodeCost<<" Connection From "<<endNodeRecord->m_connection->getFromNode()->getRegionName()<<" To "<<endNodeRecord->m_connection->getToNode()->getRegionName()<<std::endl;
                m_openList->add(endNodeRecord);
            }
        }
        //We’ve finished looking at the connections for the current node, so add it to the closed list 
        //and remove it from the open list
        std::cout<<"----remove from open list----"<<std::endl;
        m_openList->remove(current);

        std::cout<<"----add in closed list----"<<std::endl;
        m_closedList->add(current);

        std::cout<<"\n--------------------------------------------------------------------"<<std::endl;
        std::cout<<"--------------------FINAL OPEN LIST--------------------"<<std::endl;
        m_openList->printListOfNodes();
        std::cout<<"--------------------FINAL CLOSED LIST--------------------"<<std::endl;
        m_closedList->printListOfNodes();
        std::cout<<"--------------------------------------------------------------------\n"<<std::endl;
    }
    //We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
    if(current->m_node != GoalNode){
        //We’ve run out of nodes without finding the goal, so there’s no solution
        std::cout<<"!! COULDNT FIND THE PATH TO THE END NODE"<<std::endl;
        return NULL;
    }
    else{
        std::cout<<"!! FOUND THE PATH TO THE END NODE with cost "<< current->m_costSoFar<<std::endl;
        path = new std::list<Connection*>();

        while(current->previous !=0 && current->m_node!= StartNode){
            path->push_back(current->m_connection);
            current = current->previous;
        }
        path->reverse();
        return path;
    }
}

/*
        //Find the smallest element in the open list
        if(current != m_startRecord) current = m_openList->smallestElement(current->m_node);
        std::cout<<"is current node null?"<<std::endl;
        if(current == NULL) std::cout<<"yes"<<std::endl;
        else std::cout<<"No"<<std::endl;
        std::cout<<"--------------------------- CURRENT NODE "<<current->m_node->getRegionName()<<" ---------------------------"<<std::endl;

        //std::cout<<"--current CostSoFar: "<<current->m_costSoFar<<std::endl;
       // std::cout<<"--curret Connection: From < "<<current->m_connection->getFromNode()->getRegionName()<<"// To > "<<current->m_connection->getToNode()->getRegionName()<<std::endl;

        //If it is the goal node, then terminate
        if(current->m_node == GoalNode){ 
            std::cout<<"!! FOUND THE GOAL NODE "<<std::endl;
            break;
            }

        //Otherwise get its outgoing connections
        std::cout<<"Checkig outgoing connections "<<std::endl;
        nodeConnections = current->m_node->getOutgoingConnections();
        printNodeConnections(current->m_node);

        //Loop through each connection in turn
        for(int i = 0; i< nodeConnections.size();i++){
            std::cout<<"checking connection "<<i<<std::endl;
            //Get the cost estimate for the end node 
            endNode = nodeConnections.at(i)->getToNode();
            std::cout<<"This connection leads to: "<<endNode->getRegionName()<<std::endl;
            endNodeCost =  current->m_costSoFar + nodeConnections.at(i)->getCost();
            //std::cout<<"Connection "<<i<<" : endNodeCost "<<endNodeCost<<std::endl;
            //Skip if the node is closed
            if(m_closedList->contains(endNode)){ 
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the CLOSED list\n"<<std::endl;
                continue;
                }
                
            //.. or if it is open and we’ve found a worse route
            else if(m_openList->contains(endNode)){
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the OPEN list\n"<<std::endl;
                //Here we find the record in the open list corresponding to the endNode.
                endNodeRecord = m_openList->find(endNode);
                if(endNodeRecord!=NULL && endNodeRecord->m_costSoFar <= endNodeCost) continue;
            }   
            
            //Otherwise we know we’ve got an unvisited node, so make a record for it
            else{
               // std::cout<<"----------possible new node -> ACTUAL OPEN LIST----------"<<std::endl;
                m_openList->printListOfNodes();
                //NodeRecord* endNodeRecord = new NodeRecord();
                endNodeRecord->m_node = endNode;
                
                //We’re here if we need to update the node. Update the cost and connection
                endNodeRecord->m_costSoFar = endNodeCost;
                endNodeRecord->m_connection = nodeConnections.at(i);
               // std::cout<<"We have an unvisited node "<<endNode->getRegionName()<<std::endl;
                //std::cout<<"Does the open list contains it? "<<m_openList->contains(endNode)<<std::endl;
                //And add it to the open list
                std::cout<<"\n actual open list\n"<<std::endl;
                m_openList->printListOfNodes();
                if(!m_openList->contains(endNode)){
                    std::cout<<"!! Unvisited node found: NodeID: "<<endNode->getNodeID()<<" ,Region: "<<endNode->getRegionName()<<" ,NodeCostSoFar-> "<<endNodeRecord->m_costSoFar<<" ,NodeConnection: from < "<<endNodeRecord->m_connection->getFromNode()->getRegionName()<<" // To > "<<endNodeRecord->m_connection->getToNode()->getRegionName()<<"\n"<<std::endl;
                    m_openList->add(endNodeRecord);
                    std::cout<<" \n new open list!!\n"<<std::endl;
                    m_openList->printListOfNodes();
                }
            }
        }  

        //We’ve finished looking at the connections for the current node, so add it to the closed list 
        //and remove it from the open list
        m_openList->remove(current);
        m_closedList->add(current);

        std::cout<<"--------------------OPEN LIST--------------------"<<std::endl;
        m_openList->printListOfNodes();
        std::cout<<"--------------------CLOSED LIST--------------------"<<std::endl;
        m_closedList->printListOfNodes();
    }

    //We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
    if(current->m_node != GoalNode){
        //We’ve run out of nodes without finding the goal, so there’s no solution
        return NULL;
    }
    else{
        return path;
    }
*/