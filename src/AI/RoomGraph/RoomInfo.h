#ifndef ROOMINFO_H
#define ROOMINFO_H

#include <vector3d.h>
#include <iostream>
#include <vector>

// NO CAMBIAR EL ORDEN, MUY IMPORTANTE
enum direction_Code{
	DIR_UP 			= 0x00,
	DIR_UP_RIGHT 	= 0x01,
	DIR_RIGHT 		= 0x02,
	DIR_DOWN_RIGHT 	= 0x03,
	DIR_DOWN 		= 0x04,
	DIR_DOWN_LEFT 	= 0x05,
	DIR_LEFT 		= 0x06,
	DIR_UP_LEFT		= 0x07,
	DIR_SIZE		= 0x08
};

class RoomInfo{
public:
	RoomInfo(int id, vector3df pos);
	~RoomInfo();
	bool AddNextRoom(RoomInfo* next);
	void UpdateExplore(float rotY);
	float WhereExplore();
	RoomInfo* GetNextRoom();

	// GETTERS
	int GetId();
	bool GetExplored();
	vector3df GetPosition();
	std::vector<int> GetConnections();
private:
	void ShuffleVector();

	int						m_id; 				// Id de la habitacion
	vector3df 				m_position;			// Posicion central de la habitacion
	float					m_securityLevel;	// Nivel de seguridad de la habitacion
	std::vector<RoomInfo*>	m_nextRooms;		// Habitaciones contiguas de la habitacion

	bool 			m_explored[DIR_SIZE];		// Array con las direcciones exploradas	
};

#endif