#include "LoaderRoomGraph.h"

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
    	graph->AddRoom(ID, position);
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
