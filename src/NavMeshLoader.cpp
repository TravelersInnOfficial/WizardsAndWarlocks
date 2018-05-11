#include "NavMeshLoader.h"

#include "Objects/NavMesh.h"
#include <vector3d.h>
#include <json.hpp>
#include <fstream>



NavMeshLoader::NavMeshLoader(){

}

NavMeshLoader::~NavMeshLoader(){

}

void NavMeshLoader::LoadNavMeshGraph(NavMesh* navmesh, std::string jsonPath){  
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

    //trianlge data
    int v0 = 0;
    int v1 = 0;
    int v2 = 0;

	//iterates objects
	for(int i = 0; !j["NavMesh"][i].is_null(); i++){
         if(j["NavMesh"][i]["Type"] == "Node"){
            ID = j["NavMesh"][i]["ID"];
            float x = j["NavMesh"][i]["Position"]["X"];
            float y = j["NavMesh"][i]["Position"]["Y"];
            float z = j["NavMesh"][i]["Position"]["Z"];
            position = vector3df(x,y,z);
            navmesh->AddNode(ID,position);
        }   	
        else if(j["NavMesh"][i]["Type"] == "Connection"){
            fromNodeIndex = j["NavMesh"][i]["Connection"]["FromNode"];
            ToNodeIndex = j["NavMesh"][i]["Connection"]["ToNode"];
            cost = j["NavMesh"][i]["Connection"]["Cost"];
            navmesh->AddConnection(cost, fromNodeIndex, ToNodeIndex);

        }
        else if(j["NavMesh"][i]["Type"] == "Triangle"){
            v0 = j["NavMesh"][i]["Vertices"][0];
            v1 = j["NavMesh"][i]["Vertices"][1];
            v2 = j["NavMesh"][i]["Vertices"][2];
            navmesh->AddTriangle(v0,v1,v2);
        }
    }
}