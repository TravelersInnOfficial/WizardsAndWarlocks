#ifndef ROOMGRAPH_H
#define ROOMGRAPH_H

#include "./RoomInfo.h"

#include <iostream>
#include <vector>

class RoomGraph{
public:
	RoomGraph();
	RoomGraph(RoomGraph* graph);
	~RoomGraph();

	RoomInfo* AddRoom(int id, vector3df position);
	bool AddConnection(int first, int second);
	void CopyGraph(RoomGraph* copyGraph);
private:
	std::vector<RoomInfo*>	m_rooms;
};

#endif