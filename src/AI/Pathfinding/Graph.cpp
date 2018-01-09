#include "Graph.h"

Graph::Graph(){

}
Graph::~Graph(){}

void Graph::addConnection(float cost, int FromNode,int ToNode){
    Connection* c = new Connection(cost, m_nodes[FromNode], m_nodes[ToNode]);
    m_connections.push_back(c);
}
void Graph::addNode(int id, vector3df position){
    Node* n = new Node(id, position);
    m_nodes.push_back(n);
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
    return m_connections;
}

std::vector<Node*> Graph::getNodes(){
    return m_nodes;
}