#include "Graph.h"

Graph::Graph(){
    
}
Graph::~Graph(){}

void Graph::addConnection(float cost, Node* FromNode, Node* ToNode){
    Connection* c = new Connection(cost, FromNode, ToNode);
    m_connections.push_back(c);
}

void Graph::printActualGraphConnections(){
    std::cout<<"\n----Actual graph connections----"<<std::endl;
    for(int i = 0; i<m_connections.size();i++){
        //std::string fromroom = m_connections[i]->getFromNode()->getRegionName();
        //std::string toroom = m_connections[i]->getToNode()->getRegionName();
        int fromid = m_connections[i]->getFromNode()->getNodeID();
        int toid = m_connections[i]->getToNode()->getNodeID();
        std::cout<<"Connection "<<i<<" : from "<<fromid<<" to "<<toid<<std::endl;
    }
}

void Graph::printNodeConnections(Node* n){
    std::cout<<"\n----Node connections----"<<std::endl;
    std::cout<<"Node ID: "<<n->getNodeID()<<" / Node Region: "<<n->getRegionName()<<" :: \n"<<std::endl;
    std::vector<Connection*> c = n->getOutgoingConnections();
    for(int i = 0; i<c.size(); i++){
        std::cout<<"Connection "<< i <<": "<<c[i]->getFromNode()->getRegionName()<<" > "<<c[i]->getToNode()->getRegionName()<<" with cost: "<<c[i]->getCost()<<std::endl;
    }
}

std::vector<Connection*> Graph::getConnections(){
    std::cout<<"m_connections size: "<<m_connections.size()<<std::endl;
    return m_connections;
}