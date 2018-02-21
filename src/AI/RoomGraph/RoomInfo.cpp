#include "RoomInfo.h"
#include <Constants.h>
#include <limits>

RoomInfo::RoomInfo(int id, vector3df pos, vector3df firstSide, vector3df secondSide){
	m_id = id;
	m_position = pos;
	m_firstSide = firstSide;
	m_secondSide = secondSide;
	m_securityLevel = 0;
}

RoomInfo::~RoomInfo(){}

void RoomInfo::ChangeSecurityLevel(float value){
	// Alteramos el valor de seguridad de la habitacion
	m_securityLevel += value;
	if(m_securityLevel<0) m_securityLevel = 0;
	else if(m_securityLevel>100) m_securityLevel = 100;
}

void RoomInfo::AddPositionExplore(vector3df position){
	m_explored.push_back(position);
	m_statusExplored.push_back(false);
}

vector3df RoomInfo::WhereExplore(vector3df pos){
	vector3df output;

	int value = -1;
	float distance = std::numeric_limits<float>::max();

	int size = m_explored.size();
	for(int i=0; i<size; i++){
		bool explored = m_statusExplored[i];
		if(!explored){
			float currentDistance = (pos - m_explored[i]).length();
			if(currentDistance<distance){
				distance = currentDistance;
				value = i;
			}
		}
	}

	if(value!=-1){
		output = m_explored[value];
	}

	return output;
}

void RoomInfo::UpdateExplore(vector3df pos){
	int size = m_explored.size();
	for(int i=0; i<size; i++){
		bool explored = m_statusExplored[i];
		if(!explored){
			vector3df comparePos = m_explored[i];
			pos.Y = comparePos.Y;
			float distance = (comparePos-pos).length();
			if(distance<1){
				m_statusExplored[i] = true;
			}
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


std::vector<vector3df> RoomInfo::GetExplorePoints(){
	return m_explored;
}

float RoomInfo::GetSecurityLevel(){
	return m_securityLevel;
}

bool RoomInfo::GetExplored(){
	bool output = true;
	int size = m_explored.size();
	for(int i=0; i<size; i++){
		bool explored = m_statusExplored[i];
		if(!explored){
			output = false;
			break;
		}
	}
	return output;
}

float RoomInfo::NearestPoint(float pointA, float pointB, float target){
	float output = 0;
	// Vamos a asumir en un principio que pointA es menor que B
	// De esta forma comprobamos si es mentira y si lo es los cambiamos
	if(pointA>pointB){
		float aux = pointA;
		pointA = pointB;
		pointB = aux;
	}
	// Una vez ya tenemos los valores comprobamos si el target se encuenra entre los valores
	if(target>pointA && target<pointB){
		output = target;
	}
	// En el caso de que no este entre los valores veremos si se pasa por el valor mayor
	else if(target > pointB){
		output = pointB;
	}
	// Por descarte ya solamente queda el caso de que se pase de pequenyo
	else{
		output = pointA;
	}

	return output;
}

float RoomInfo::GetDistance(vector3df target){
	// Calculamos el punto mas cercano al target de la habitacion
	float xPos = NearestPoint(m_firstSide.X, m_secondSide.X, target.X);
	float zPos = NearestPoint(m_firstSide.Z, m_secondSide.Z, target.Z);

	// Formamos un vector con los valores, e igualamos la altura de ambos vectores
	// En este caso no nos interesa tener en cuenta la altura 
	vector3df nearPoint(xPos, 0, zPos);
	target.Y = 0;

	// Calculamos la distancia de un punto a otro
	float output = (target - nearPoint).length();
	
	return output;
}

int RoomInfo::GetId(){
	return m_id;
}

vector3df RoomInfo::GetPosition(){
	return m_position;
}


vector3df RoomInfo::GetFirstSide(){
	return m_firstSide;
}

vector3df RoomInfo::GetSecondSide(){
	return m_secondSide;
}


RoomInfo* RoomInfo::GetNextRoom(){
	ShuffleVector();
	int size = m_nextRooms.size();
	for(int i=0; i<size; i++){
		bool explored = m_nextRooms[i]->GetExplored();
		if(!explored){
			return m_nextRooms[i];
		}
	}
	return NULL;
}

void RoomInfo::ShuffleVector(){
	int n = m_nextRooms.size();
	while(n>1){
		int k = rand() % n;
		n--;
		RoomInfo* temp = m_nextRooms[k];
		m_nextRooms[k] = m_nextRooms[n];
		m_nextRooms[n] = temp;
	}
}