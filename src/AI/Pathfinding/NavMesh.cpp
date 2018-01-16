#include "NavMesh.h"

NavMesh::NavMesh(){
    /*
    m_nodes =  new std::vector<Node*>();
    m_connections = new std::vector<Connection*>();
    m_triangles = new std::vector<Triangle*>();
    */
    m_graph = new Graph();
}

NavMesh::NavMesh(std::vector<Node*> nodes, std::vector<Connection*> connections, std::vector<Triangle*> triangles){
    m_nodes = nodes;
    m_connections = connections;
    m_triangles = triangles;
    m_graph = new Graph();
}

NavMesh::~NavMesh(){
    /*
    for(int i = 0; i<m_connections->size();i++){
        delete m_connections->at(i);
    }
    for(int i = 0; i<m_triangles->size();i++){
        delete m_triangles->at(i);
    }
    for(int i = 0; i<m_nodes->size();i++){
        delete m_nodes->at(i);
    }*/
    //
    //delete m_connections;
    //delete m_triangles;
    //delete m_graph;
}

std::vector<Node*> *NavMesh::searchNearestNodes(vector3df point){
    bool flag = false;
    std::vector<vector3df> tri;
    std::vector<Node*> *arr = new std::vector<Node*>();
    for(int i = 0;i<m_triangles.size();i++){
        tri.push_back(m_triangles[i]->vertices[0]->getPosition());
        tri.push_back(m_triangles[i]->vertices[1]->getPosition());
        tri.push_back(m_triangles[i]->vertices[2]->getPosition());
        flag = pointInTriangle(point,tri);
        if(flag){ 
            arr->push_back(m_triangles[i]->vertices[0]);
            arr->push_back(m_triangles[i]->vertices[1]);
            arr->push_back(m_triangles[i]->vertices[2]);
            return arr;
        }
    }
    return NULL;
}

bool NavMesh::pointInTriangle(vector3df point, std::vector<vector3df> triangle){
    /*
    A = triangle[0]
    B = triangle[1]
    C = triangle[2]
    p = point
    Any point p where [B-A] cross [p-A] does not point in the same direction
    as [B-A] cross [C-A] isn't inside the triangle. 
    If the cross products do point in the same direction, then we need to test p with the other lines
    as well. If the point was on the same side of AB as C and is also on the same side
    of BC as A and on the same side of CA as B, then it is in the triangle.
    */
    if(sameSide(point, triangle[0], triangle[1], triangle[2]) 
    && sameSide(point, triangle[1], triangle[0], triangle[2])
    && sameSide(point, triangle[2], triangle[0], triangle[1])) return true;
    return false;
}

float NavMesh::dotProduct(vector3df A, vector3df B){
    //A*B = A.X*B.X + A.Y*B.Y + A.Z*B.Z
    return (A.X*B.X + A.Y*B.Y + A.Z*B.Z);   
}

vector3df NavMesh::crossProduct(vector3df A, vector3df B){
    //A = [A.X A.Y A.Z]
    //B = [B.X B.Y B.Z]
    //AxB = [A.X*B.Y - A.Y*B.X, A.Y*B.Z - A.Z*B.Y, A.Z*B.X - A.X*B.Z]
    return vector3df(A.X*B.Y - A.Y*B.X, A.Y*B.Z - A.Z*B.Y, A.Z*B.X - A.X*B.Z);
}

bool NavMesh::sameSide(vector3df p1, vector3df p2, vector3df A, vector3df B){
    vector3df cp1 = crossProduct(vector3df(B.X-A.X,B.Y-A.Y,B.Z-A.Z),vector3df(p1.X-A.X,p1.Y-A.Y,p1.Z-A.Z));
    vector3df cp2 = crossProduct(vector3df(B.X-A.X,B.Y-A.Y,B.Z-A.Z),vector3df(p2.X-A.X,p2.Y-A.Y,p2.Z-A.Z));
    if(dotProduct(cp1,cp2) >= 0) return true;
    else return false;
}

std::vector<Triangle*> NavMesh::getTriangles(){
    return m_triangles;
}
    
Graph NavMesh::getGraph(){
    return *m_graph;
}

void NavMesh::printData(){
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
