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
	return nullptr;
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

// En el caso de que ambos jugadores esten en la misma habitacion, el jugador debera ir
// a la habitacion mas cercana que tenga sin pasar por detras del enemigo
vector3df RoomInfo::GetEscapeRoomSameRoom(vector3df player, vector3df target){
	// Creamos un vector3df que sera el que devolvamos en caso de no encontrar ningun valor
	vector3df output;

	// Creamos los valores iniciales de distancia y valor, inicializados a la maxima distancia posible
	float distance = std::numeric_limits<float>::max();
	int value = -1;

	// Miramos habitacion a habitacion si la distancia es menor
	int size = m_nextRooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_nextRooms[i];
		// Miramos la distancia del player y del target a la habitacion
		float currentLengthTarget = info->GetDistance(target);
		float currentLengthPlayer = info->GetDistance(player);

		// Miramos si es valido: el player tiene mas cerca la habitacion que el player
		if(currentLengthPlayer<currentLengthTarget){
			// Nos quedamos con la habiacion mas cercana al player de entre las validas
			if(currentLengthPlayer<distance){
				distance = currentLengthPlayer;
				value = i;
			}
		}

	}

	// Miramos si hemos llegado a encontrar algun valor
	if(value != -1){
		// Igualamos el valor al centro de la habitacion mas cercana para escapar
		output = m_nextRooms[value]->GetPosition();
	}else if(size>=1){
		// En el caso de no encontrar habitacion devolvemos la primera salida
		output = m_nextRooms[0]->GetPosition();
	}else{
		// Ya en el caso de que no tenga habitaciones conectadas devolvemos el centro de esta
		output = m_position;
	}
	// Devolvemos el valor al que ir
	return output;
}

// En el caso de que ambos jugadores esten en diferentes habitaciones, el jugador ira a la habitacion mas lejana
// al target
vector3df RoomInfo::GetEscapeRoomDifferentRoom(vector3df player, vector3df target){
	// Creamos un vector3df que sera el que devolvamos en caso de no encontrar ningun valor
	vector3df output;

	// Creamos los valores iniciales de distancia y valor
	float distance = -1;
	int value = -1;

	// Miramos habitacion a habitacion si la distancia es mayor
	int size = m_nextRooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_nextRooms[i];


		vector3df oneSide = info->GetFirstSide();
		vector3df secondSide = info->GetSecondSide();

		// Comprobamos que el player no se encuentre en esta habitacion
		bool insideX = (target.X < oneSide.X && target.X > secondSide.X) 
					|| (target.X > oneSide.X && target.X < secondSide.X);
		bool insideZ = (target.Z < oneSide.Z && target.Z > secondSide.Z) 
					|| (target.Z > oneSide.Z && target.Z < secondSide.Z);

		if(!(insideX && insideZ)){
		// Miramos la distancia del target a la habitacion
			float currentLengthTarget = info->GetDistance(target);
			if(currentLengthTarget>distance){
				distance = currentLengthTarget;
				value = i;
			}
		}
	}

	// Miramos si hemos llegado a encontrar algun valor
	if(value != -1){
		output = m_nextRooms[value]->GetPosition();
	}else{
		// En el caso de no encontrar habitacion
		// Enviamos un vector3df con "booleanos"
		output = vector3df(std::numeric_limits<float>::max());
	}
	return output;
}

// Meter comparatiba de si el target esta en la misma habitacion
vector3df RoomInfo::GetEscapeRoom(vector3df player, vector3df target){
	// Comprobamos si el target esta dentro de la misma habitacion que el player
	bool insideX = (target.X < m_firstSide.X && target.X > m_secondSide.X) 
					|| (target.X > m_firstSide.X && target.X < m_secondSide.X);
	bool insideZ = (target.Z < m_firstSide.Z && target.Z > m_secondSide.Z) 
					|| (target.Z > m_firstSide.Z && target.Z < m_secondSide.Z);

	// Creamos el valor de output
	vector3df output;
	if(insideX && insideZ){
		output = GetEscapeRoomSameRoom(player, target);
	}else{
		output = GetEscapeRoomDifferentRoom(player, target);
	}
	return output;
}