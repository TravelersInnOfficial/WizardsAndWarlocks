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
	RoomInfo* AddRoom(int id, vector3df position, vector3df firstSide, vector3df secondSide);
	bool AddConnection(int first, int second);
	
	void CopyGraph(RoomGraph* copyGraph);

	// ACTUAL ROOMINFO
	vector3df RoomPos();
	bool RoomExplored();
	vector3df WhereExplore(vector3df pos);
	void UpdateExplore(vector3df pos);
	vector3df GetFirstCorner();
	vector3df GetSecondCorner();

	// NEXT ROOM
	RoomInfo* GetUnexploredRoom();
	bool NextRoom();
	vector3df NextRoomPos();
private:
	void ShuffleVector();
	bool CheckInside(float A, float B, float C);

	std::vector<RoomInfo*>	m_rooms;
	RoomInfo*				m_actualRoom;
	RoomInfo*				m_nextRoom;
};

#endif