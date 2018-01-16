#include "NavMeshLoader.h"
#include <fstream>
#include <vector3d.h>
#include <json.hpp>

NavMeshLoader::NavMeshLoader(){

}

NavMeshLoader::~NavMeshLoader(){

}

NavMesh NavMeshLoader::LoadNavMeshGraph(std::string jsonPath){  
    std::vector<Node*> nodes;
    std::vector<Connection*> connections;
    std::vector<Triangle*> triangles;

    //Takes path from binary location (/bin)
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

    //node data
    int ID = 0;
    vector3df position;

    //connection data
    int fromNodeIndex = 0;
    int ToNodeIndex = 0;
    float cost = 0;

	//iterates objects
	for(int i = 0; !j["NavMesh"][i].is_null(); i++){
         if(j["NavMesh"][i]["Type"] == "Node"){
            ID = j["NavMesh"][i]["ID"];
            float x = j["NavMesh"][i]["Position"]["X"];
            float y = j["NavMesh"][i]["Position"]["Y"];
            float z = j["NavMesh"][i]["Position"]["Z"];
            position = vector3df(x,y,z);
            Node* n = new Node(ID,position);
            nodes.push_back(n);
        }   	
        else if(j["NavMesh"][i]["Type"] == "Connection"){
            fromNodeIndex = j["NavMesh"][i]["Connection"]["FromNode"];
            ToNodeIndex = j["NavMesh"][i]["Connection"]["ToNode"];
            cost = j["NavMesh"][i]["Connection"]["Cost"];
            Connection* c = new Connection(cost,nodes[fromNodeIndex], nodes[ToNodeIndex]);
            connections.push_back(c);
        }
        else if(j["NavMesh"][i]["Type"] == "Triangle"){
            int v0 = j["NavMesh"][i]["Vertices"][0];
            int v1 = j["NavMesh"][i]["Vertices"][1];
            int v2 = j["NavMesh"][i]["Vertices"][2];
            Triangle *tri = new Triangle();
            tri->vertices.push_back(nodes[v0]);
            tri->vertices.push_back(nodes[v1]);
            tri->vertices.push_back(nodes[v2]);

            triangles.push_back(tri);
        }
    }
    NavMesh navmesh(nodes, connections, triangles);
    return navmesh;
}