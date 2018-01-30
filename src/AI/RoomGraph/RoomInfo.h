#ifndef ROOMINFO_H
#define ROOMINFO_H

#include <vector3d.h>
#include <iostream>
#include <vector>

class RoomInfo{
public:
	RoomInfo(int id, vector3df pos);
	~RoomInfo();
	bool AddNextRoom(RoomInfo* next);
	int GetId();
	vector3df GetPosition();
	std::vector<int> GetConnections();
private:
	int						m_id; 				// Id de la habitacion
	vector3df 				m_position;			// Posicion central de la habitacion
	float					m_securityLevel;	// Nivel de seguridad de la habitacion
	std::vector<RoomInfo*>	m_nextRooms;		// Habitaciones contiguas de la habitacion
};

#endif