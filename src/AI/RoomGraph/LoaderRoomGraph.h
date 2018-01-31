#ifndef LOADERROOMGRAPH_H
#define LOADERROOMGRAPH_H

#include "RoomGraph.h"

#include <fstream>
#include <iostream>
#include <json.hpp>

class LoaderRoomGraph{
public:
	LoaderRoomGraph();
	~LoaderRoomGraph();

	static void LoadRoomGraph(RoomGraph* graph, std::string path);

private:
};


#endif