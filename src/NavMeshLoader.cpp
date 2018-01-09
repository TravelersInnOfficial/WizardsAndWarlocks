#include "NavMeshLoader.h"
#include <fstream>
#include <vector3d.h>
#include <json.hpp>

NavMeshLoader::NavMeshLoader(){

}

NavMeshLoader::~NavMeshLoader(){

}

Graph NavMeshLoader::LoadNavMeshGraph(std::string jsonPath){  
    Graph graph;

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
            graph.addNode(ID,position);
        }   	
        else if(j["NavMesh"][i]["Type"] == "Connection"){
            fromNodeIndex = j["NavMesh"][i]["Connection"]["FromNode"];
            ToNodeIndex = j["NavMesh"][i]["Connection"]["ToNode"];
            cost = j["NavMesh"][i]["Connection"]["Cost"];
            graph.addConnection(cost, fromNodeIndex, ToNodeIndex);
        }
    }

    return graph;
}