#include "RoomInfo.h"

RoomInfo::RoomInfo(int id, vector3df pos){
	m_id = id;
	m_position = pos;
	m_securityLevel = 0;
}

RoomInfo::~RoomInfo(){}

bool RoomInfo::AddNextRoom(RoomInfo* next){
	int size = 0;
	for(int i=0; i<size; i++){
		RoomInfo* room = m_nextRooms[i];
		if(room == next){	// Comprobamos que no estuviera anyadida ya la habitacion
			return false;
		}
	}
	m_nextRooms.push_back(next);
	return true;
}

std::vector<int> RoomInfo::GetConnections(){
	std::vector<int> output;
	int size = m_nextRooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_nextRooms[i];
		output.push_back(info->GetId());
	}
	return output;
}

int RoomInfo::GetId(){
	return m_id;
}

vector3df RoomInfo::GetPosition(){
	return m_position;
}