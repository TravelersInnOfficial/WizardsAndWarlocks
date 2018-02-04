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

	void InitRoom(vector3df pos);
	void UpdateExplore(float rotY);

	RoomInfo* AddRoom(int id, vector3df position);
	bool AddConnection(int first, int second);
	bool RoomExplored();
	void CopyGraph(RoomGraph* copyGraph);
private:
	std::vector<RoomInfo*>	m_rooms;
	RoomInfo*				m_actualRoom;
};

#endif