#ifndef LOADERROOMGRAPH_H
#define LOADERROOMGRAPH_H

#include <iostream>

class RoomGraph;

class LoaderRoomGraph{
public:
	LoaderRoomGraph();
	~LoaderRoomGraph();

	static void LoadRoomGraph(RoomGraph* graph, std::string path);

private:
};


#endif