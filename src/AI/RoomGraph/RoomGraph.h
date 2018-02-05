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
	RoomInfo* AddRoom(int id, vector3df position);
	bool AddConnection(int first, int second);
	
	void CopyGraph(RoomGraph* copyGraph);

	// ACTUAL ROOMINFO
	vector3df RoomPos();
	bool RoomExplored();
	float WhereExplore();
	void UpdateExplore(float rotY);

	// NEXT ROOM
	void NextRoom();
	vector3df NextRoomPos();
private:
	void ShuffleVector();

	std::vector<RoomInfo*>	m_rooms;
	RoomInfo*				m_actualRoom;
	RoomInfo*				m_nextRoom;
};

#endif