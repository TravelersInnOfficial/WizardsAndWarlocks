#include "LoaderRoomGraph.h"

#include "RoomGraph.h"
#include "RoomInfo.h"
#include <fstream>
#include <iostream>
#include <json.hpp>

LoaderRoomGraph::LoaderRoomGraph(){}

LoaderRoomGraph::~LoaderRoomGraph(){}

void LoaderRoomGraph::LoadRoomGraph(RoomGraph* graph, std::string path){
	 //Takes path from binary location (/bin)
	std::ifstream jsonPath(path);
	nlohmann::json j;
	jsonPath >> j;

	// Create all the rooms
	for(int i = 0; !j["Rooms"][i].is_null(); i++){
        // Prepare the data for create the new room
        int ID 	= j["Rooms"][i]["ID"];
        float x = j["Rooms"][i]["POSITION_X"];
        float y = j["Rooms"][i]["POSITION_Y"];
    	float z = j["Rooms"][i]["POSITION_Z"];
    	vector3df position(x, y, z);
        x = j["Rooms"][i]["F_SIDE_X"];
        y = j["Rooms"][i]["F_SIDE_Y"];
        z = j["Rooms"][i]["F_SIDE_Z"];
        vector3df firstSide(x, y, z);
        x = j["Rooms"][i]["S_SIDE_X"];
        y = j["Rooms"][i]["S_SIDE_Y"];
        z = j["Rooms"][i]["S_SIDE_Z"];
        vector3df secondSide(x, y, z);
    	RoomInfo* info = graph->AddRoom(ID, position, firstSide, secondSide);
        // Load all the points to explore inside the rooms
        for(int k=0; !j["Rooms"][i]["EXPLORE"][k].is_null(); k++){
            x = j["Rooms"][i]["EXPLORE"][k]["POSITION_X"];
            y = j["Rooms"][i]["EXPLORE"][k]["POSITION_Y"];
            z = j["Rooms"][i]["EXPLORE"][k]["POSITION_Z"];
            vector3df explorePoint(x, y, z);
            info->AddPositionExplore(explorePoint);
        }
    }	

    // Create all the connections
    for(int i=0; !j["Connections"][i].is_null(); i++){
    	int ID_first = j["Connections"][i]["ID"];

    	for(int k=0; !j["Connections"][i]["CONNECTION"][k].is_null(); k++){
    		// Prepare the data for create the connections
    		int ID_second = j["Connections"][i]["CONNECTION"][k];
    		graph->AddConnection(ID_first, ID_second);
    	}
    }
}
