#include "RoomInfo.h"

RoomInfo::RoomInfo(int id, vector3df pos){
	m_id = id;
	m_position = pos;
	m_securityLevel = 0;

	for(int i=0; i<DIR_SIZE; i++){
		m_explored[i] = false;
	}
}

RoomInfo::~RoomInfo(){}

float RoomInfo::WhereExplore(){
	float output = 0;
	float pi_4 = 2*M_PI/8;

	float firstValue;
	for(int i=0; i<DIR_SIZE; i++){
		firstValue = pi_4 * i;
		if(!m_explored[i]){
			output = firstValue;
			break;
		}
	}
	return output;
}

void RoomInfo::UpdateExplore(float rotY){
	float pi_4 = 2*M_PI/8;
	float pi_8 = 2*M_PI/16;

	rotY += pi_8;	// Movemos la rotacion Y para que se ha más facil hacer las comprobaciones

	if(rotY >= 2*M_PI) rotY -= 2*M_PI;	// Haceos que el valor este entre [0, 2PI] para las comprobaciones
	else if(rotY<0) rotY += 2*M_PI;

	// Rotacion Y del personaje en Radianes
	float firstValue, secondValue;
	for(int i=0; i<DIR_SIZE; i++){		// El primer valor que se comprueba es el UP_LEFT
		firstValue = pi_4*i;
		secondValue = firstValue + pi_4;
		if(rotY > firstValue && rotY < secondValue){	// Si el vlor se encuentra entre los rangos poner el valor a true
			m_explored[i] = true;	// Lado explorado
			break;
		}
	}

}

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

bool RoomInfo::GetExplored(){
	bool output = true;
	for(int i=0; i<DIR_SIZE; i++){
		if(!m_explored[i]){
			output = false;
			break;
		}
	}
	return output;
}

int RoomInfo::GetId(){
	return m_id;
}

vector3df RoomInfo::GetPosition(){
	return m_position;
}