#include "NavMesh.h"

NavMesh::NavMesh(){

}

NavMesh::NavMesh(std::vector<Node*> nodes, std::vector<Connection*> connections, std::vector<Triangle*> triangles){
    m_nodes = nodes;
    m_connections = connections;
    m_triangles = triangles;
}

NavMesh::~NavMesh(){
    for(int i=m_connections.size()-1; i>=0; i--){
        m_connections.erase(m_connections.begin()+i);
    }
    m_connections.clear();

    for(int i=m_triangles.size()-1; i>=0; i--){
        m_triangles.erase(m_triangles.begin()+i);
    }
    m_triangles.clear();

    for(int i=m_nodes.size()-1; i>=0; i--){
        m_nodes.erase(m_nodes.begin()+i);
    }
    m_nodes.clear();
}

void NavMesh::addNode(int id, vector3df position){
    Node* n =  new Node(id,position);
    m_nodes.push_back(n);
}

void NavMesh::addConnection(float cost, int from_index, int to_index){
    Node *from = m_nodes[from_index];
    Node *to = m_nodes[to_index];
    Connection* c = new Connection(cost, from, to);
    m_connections.push_back(c);
}

void NavMesh::addTriangle(int vertex1, int vertex2, int vertex3){
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
std::vector<Node*> NavMesh::searchNearestNodes(vector3df point){
    bool flag = false;
    std::vector<vector3df> tri;
    std::vector<Node*> arr;

    int i = 0;
    for(;i<m_triangles.size();i++){
        vector3df v1 = m_triangles[i]->vertices[0]->getPosition();
        vector3df v2 = m_triangles[i]->vertices[1]->getPosition();
        vector3df v3 = m_triangles[i]->vertices[2]->getPosition();

        tri.push_back(v1);
        tri.push_back(v2);
        tri.push_back(v3);

        flag = pointInTriangle(point,tri);
            if(flag){ 
                Node *n1 = m_triangles[i]->vertices[0];                
                Node *n2 = m_triangles[i]->vertices[1];
                Node *n3 = m_triangles[i]->vertices[2];
                arr.push_back(n1);
                arr.push_back(n2);
                arr.push_back(n3);
                break;
            }
            tri.clear();
        }
    return arr;
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

    bool ss_ABC = sameSide(point, triangle[0], triangle[1], triangle[2]);
    bool ss_BAC = sameSide(point, triangle[1], triangle[0], triangle[2]);
    bool ss_CAB = sameSide(point, triangle[2], triangle[0], triangle[1]);
    if(ss_ABC && ss_BAC && ss_CAB) return true;
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
