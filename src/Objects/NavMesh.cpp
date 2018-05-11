#include "NavMesh.h"

#include <limits>
#include "./../AI/Pathfinding/Node.h"
#include <GraphicEngine/GraphicEngine.h>
#include "./../AI/Pathfinding/Connection.h"



NavMesh::NavMesh(){

}

NavMesh::NavMesh(std::vector<Node*> nodes, std::vector<Connection*> connections, std::vector<Triangle*> triangles){
    m_nodes = nodes;
    m_connections = connections;
    m_triangles = triangles;
}

NavMesh::~NavMesh(){
    int size = m_connections.size();
    for(int i = size-1; i>=0; i--){
        m_connections.erase(m_connections.begin() + i);
    }
    m_connections.clear();

    size = m_triangles.size();
    for(int i = size-1; i>=0; i--){
        delete m_triangles[i];
    }
    m_triangles.clear();

    size = m_nodes.size();
    for(int i = size-1; i>=0; i--){
        delete m_nodes[i];
    }
    m_nodes.clear();
}

void NavMesh::Draw(){
    GraphicEngine* g_engine = GraphicEngine::getInstance();
    
    int size = m_nodes.size();
    for(int i=0; i<size; i++){
        vector3df position = m_nodes[i]->getPosition();
        g_engine->addCube2Scene(position, vector3df(0,0,0), vector3df(1,1,1), 0.2, i);
    }
}

void NavMesh::AddNode(int id, vector3df position){
    Node* n =  new Node(id,position);
    m_nodes.push_back(n);
}

void NavMesh::AddConnection(float cost, int from_index, int to_index){
    Node *from = m_nodes[from_index];
    Node *to = m_nodes[to_index];
    Connection* c = new Connection(cost, from, to);
    m_connections.push_back(c);
}

void NavMesh::AddTriangle(int vertex1, int vertex2, int vertex3){
    Node *v1 = m_nodes[vertex1];
    Node *v2 = m_nodes[vertex2];
    Node *v3 = m_nodes[vertex3];
    Triangle *t = new Triangle();
    t->vertices.push_back(v1);
    t->vertices.push_back(v2);
    t->vertices.push_back(v3);
    m_triangles.push_back(t);
}

// Si devuelve un puntero al una variable que se crea aqui, al salir del metodo se podria perder el valor
std::vector<Node*> NavMesh::SearchNearestNodes(vector3df point){
    std::vector<Node*> arr;

    float compare = std::numeric_limits<float>::max();

    float value = 0;
    int nodeValue = -1;

    int size = m_nodes.size();
    for(int i=0; i<size; i++){
        value = (m_nodes[i]->getPosition() - point).length();
        if(value<compare){
            compare = value;
            nodeValue = i;
        }
    }
    if(nodeValue != -1){
        arr.push_back(m_nodes[nodeValue]);
    }
    return arr;
}

std::vector<Node*> NavMesh::SearchNearestNodes(vector3df point, vector3df firstC, vector3df secondC){
    std::vector<Node*> arr;
    float compare = std::numeric_limits<float>::max();

    float value = 0;
    int nodeValue = -1;

    int size = m_nodes.size();
     for(int i=0; i<size; i++){
        value = (m_nodes[i]->getPosition() - point).length();
        if(value<compare && CheckInside(firstC.X, secondC.X, point.X) && CheckInside(firstC.Z, secondC.Z, point.Z)){
            compare = value;
            nodeValue = i;
        }
    }

    if(nodeValue != -1){
        arr.push_back(m_nodes[nodeValue]);
    }

    return arr;
}

bool NavMesh::CheckInside(float A, float B, float C){
    float output = false;
    if((C<A && C>B) || (C>A && C<B)){
        output = true;
    }
    return output;
}

std::vector<Node*> NavMesh::GetNodes(){
    return m_nodes;
}

std::vector<Connection*> NavMesh::GetConnections(){
    return m_connections;
}

std::vector<Triangle*> NavMesh::GetTriangles(){
    return m_triangles;
}

void NavMesh::PrintData(){
    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"-------------------NODES---------------------"<<std::endl;
    std::cout<<"---------------------------------------------"<<std::endl;
    for(int i = 0; i<m_nodes.size();i++){
        int id = m_nodes[i]->getNodeID();
        vector3df position = m_nodes[i]->getPosition();
        std::cout<<"Node "<<id<<": ("<<position.X<<","<<position.Y<<","<<position.Z<<")"<<std::endl;

    }

    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"----------------CONNECTIONS------------------"<<std::endl;
    std::cout<<"---------------------------------------------"<<std::endl;
    for(int i = 0; i<m_connections.size();i++){
        int fromid = m_connections[i]->getFromNode()->getNodeID();
        int toid = m_connections[i]->getToNode()->getNodeID();
        std::cout<<"Connection "<<i<<" : from "<<fromid<<" to "<<toid<<std::endl;
    }

    std::cout<<"---------------------------------------------"<<std::endl;
    std::cout<<"-----------------TRIANGLES-------------------"<<std::endl;
    std::cout<<"---------------------------------------------"<<std::endl;
    for(int i=0; i<m_triangles.size();i++){
        Node *n1 = m_triangles[i]->vertices[0];
        Node *n2 = m_triangles[i]->vertices[1];
        Node *n3 = m_triangles[i]->vertices[2];
        vector3df pos1 = n1->getPosition();
        vector3df pos2 = n2->getPosition();
        vector3df pos3 = n3->getPosition();
        std::cout<<"Triangle "<<i<<": "<<std::endl;
        std::cout<<"   "<<n1->getNodeID()<<": ("<<pos1.X<<","<<pos1.Y<<","<<pos1.Z<<")"<<std::endl;
        std::cout<<"   "<<n2->getNodeID()<<": ("<<pos2.X<<","<<pos2.Y<<","<<pos2.Z<<")"<<std::endl;
        std::cout<<"   "<<n3->getNodeID()<<": ("<<pos3.X<<","<<pos3.Y<<","<<pos3.Z<<")"<<std::endl;
    }
}
