#ifndef ROOMINFO_H
#define ROOMINFO_H

#include <vector3d.h>
#include <iostream>
#include <vector>

class RoomInfo{
public:
	RoomInfo(int id, vector3df pos, vector3df firstSide, vector3df secondSide);
	~RoomInfo();
	bool AddNextRoom(RoomInfo* next);
	void UpdateExplore(vector3df pos);
	vector3df WhereExplore(vector3df pos);
	RoomInfo* GetNextRoom();
	void AddPositionExplore(vector3df position);
	void ChangeSecurityLevel(float value);

	bool IntoSameRoom(vector3df pos);
	bool IntoNextRoom(vector3df pos);

	// GETTERS
	int GetId();
	bool GetExplored();
	float GetDistance(vector3df target);
	float GetSecurityLevel();
	vector3df GetFirstSide();
	vector3df GetSecondSide();
	vector3df GetPosition();
	std::vector<int> GetConnections();
	std::vector<vector3df> GetExplorePoints();
	vector3df GetEscapeRoom(vector3df player, vector3df target);
private:
	vector3df GetEscapeRoomSameRoom(vector3df player, vector3df target);
	vector3df GetEscapeRoomDifferentRoom(vector3df player, vector3df target);

	void ShuffleVector();
	float NearestPoint(float pointA, float pointB, float target);

	int						m_id; 				// Id de la habitacion
	vector3df 				m_position;			// Posicion central de la habitacion
	vector3df				m_firstSide;		// Posicion de la primera esquina de la habitacion
	vector3df				m_secondSide;		// Posicion de la segunda esquina de la habitacion
	float					m_securityLevel;	// Nivel de seguridad de la habitacion [0, 100] inseguro/seguro
	std::vector<RoomInfo*>	m_nextRooms;		// Habitaciones contiguas de la habitacion

	std::vector<vector3df> 	m_explored;			// Array con posiciones que explorar de la habitacion
	std::vector<bool> 		m_statusExplored;	// Array con el estado de exploracion de cada posicion

	int						m_exploreSecurity;	// Posicion del vector de puntos a explorar, el cual iremos en el caso de que todo este explorado y no este al nivel maximo de seguridad
};

#endif