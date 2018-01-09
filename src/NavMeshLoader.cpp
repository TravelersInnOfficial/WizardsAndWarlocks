#include "NavMeshLoader.h"
#include <json.hpp>
#include <fstream>
#include <vector3d.h>

NavMeshLoader::NavMeshLoader(){

}

NavMeshLoader::~NavMeshLoader(){

}

bool NavMeshLoader::LoadNavMesh(std::string jsonPath){
    //Takes path from binary location (/bin)
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

    //node data
    Node* node;
    int ID = 0;
    std::string name = "";
    vector3df position;

    //connection data
    Connection connection;
    int fromNodeIndex = 0;
    int ToNodeIndex = 0;
    float cost = 0;

    //pointer to object body
	//auto ptr;

	//iterates objects
	for(int i = 0; !j["NavMesh"][i].is_null(); i++){	
        if(j["NavMesh"][i]["Type"] == "Node"){
            ID = j["NavMesh"][i]["ID"];
            float x = j["NavMesh"][i]["Position"]["X"];
            float y = j["NavMesh"][i]["Position"]["Y"];
            float z = j["NavMesh"][i]["Position"]["Z"];
            position = vector3df(x,y,z);
            node = new Node(ID,name,position);
            nodesArray.push_back(node);
            
        }
        else if(j["NavMesh"][i]["Type"] == "Connection"){
            fromNodeIndex = j["NavMesh"][i]["Connection"]["FromNode"];
            ToNodeIndex = j["NavMesh"][i]["Connection"]["ToNode"];
            cost = j["NavMesh"][i]["Connection"]["Cost"];
            graph.addConnection(cost, nodesArray[fromNodeIndex], nodesArray[ToNodeIndex]);
        }
    }
    graph.printActualGraphConnections();

    return false;
}

std::vector<Node*> NavMeshLoader::getNavMeshNodes(){
    return nodesArray;
}
Graph NavMeshLoader::getNavMeshGraph(){
    return graph;
}

std::vector<Connection*> NavMeshLoader::getNavMeshConnections(){
    return graph.getConnections();
}