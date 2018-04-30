#include "RoomGraph.h"

#include <limits>

RoomGraph::RoomGraph(){
	m_actualRoom = nullptr;
	m_nextRoom = nullptr;
}

RoomGraph::RoomGraph(RoomGraph* graph){
	graph->CopyGraph(this);
	m_actualRoom = nullptr;
	m_nextRoom = nullptr;
}

RoomGraph::~RoomGraph(){
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		delete info;
	}
	m_rooms.clear();
}

void RoomGraph::ChangeSecurityLevel(float value){
	if(m_actualRoom != nullptr){
		m_actualRoom->ChangeSecurityLevel(value);
	}
}

RoomInfo* RoomGraph::AddRoom(int id, vector3df position, vector3df firstSide, vector3df secondSide){
	RoomInfo* room = new RoomInfo(id, position, firstSide, secondSide);
	m_rooms.push_back(room);
	return room;
}

bool RoomGraph::AddConnection(int first, int second){
	RoomInfo* firstRoom = nullptr;
	RoomInfo* secondRoom = nullptr;
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		// Cogemos los datos importante de la habitacion I
		RoomInfo* info = m_rooms[i];
		int infoId = info->GetId();

		// Miramos si alguno de las habitacion tiene la id
		if(infoId == first){
			firstRoom = m_rooms[i];
		}
		else if(infoId == second){
			secondRoom = m_rooms[i];
		}

		// Si hemos encontrado ambas habitaciones salimos
		if(firstRoom != nullptr && secondRoom != nullptr){
			break;
		}
	}
	// Comprobamos si hemos acabado el bucle con los dos valores o sin alguno de ellos
	if(firstRoom!=nullptr && secondRoom!=nullptr){
		firstRoom->AddNextRoom(secondRoom);
		return true;
	}
	// No se anyade la conexion, puesto que no se han encontrado los elementos
	return false;
}

bool RoomGraph::RoomExplored(){
	bool output = false;
	if(m_actualRoom != nullptr) output = m_actualRoom->GetExplored();
	return output;
}

vector3df RoomGraph::RoomPos(){
	vector3df output;
	if(m_actualRoom != nullptr) output = m_actualRoom->GetPosition();
	return output;
}

void RoomGraph::CopyGraph(RoomGraph* copyGraph){
	// Copiar todas las habitaciones
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		RoomInfo* copyRoom = copyGraph->AddRoom(info->GetId(), info->GetPosition(), info->GetFirstSide(), info->GetSecondSide());
		
		//Copiar el vector de posiciones del vector
		std::vector<vector3df> positions = info->GetExplorePoints();
		int sizeExplore = positions.size();
		for(int j=0; j<sizeExplore; j++){
			// Anyadimos el punto de exploracion de la habitacion
			copyRoom->AddPositionExplore(positions[j]);
		}
	}

	// Copiar todos los enlaces
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		std::vector<int> infoConnections = info->GetConnections();

		int sizeConnections = infoConnections.size();
		for(int j=0; j<sizeConnections; j++){
			copyGraph->AddConnection(info->GetId(), infoConnections[j]);
		}
	}
}

bool RoomGraph::CheckInside(float A, float B, float C){
	bool output = false;
	if((C<A && C>B) || (C>A && C<B)){
		output = true;
	}
	return output;
}

void RoomGraph::InitRoom(vector3df pos, float deltaTime){
	// Recorremos todas las habitaciones que hay en el vector
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		if(info != m_actualRoom){
			// Conseguimos las dos esquinas de cada habitacion
			vector3df firstSide = info->GetFirstSide();
			vector3df secondSide = info->GetSecondSide();
			// Miramos si la posicion pasado por parametros esta dentro de las dos esquinas
			if(CheckInside(firstSide.X, secondSide.X, pos.X) && CheckInside(firstSide.Z, secondSide.Z, pos.Z)){
				m_actualRoom = m_rooms[i];
				break;	
			}
		}
	}
	// En el caso de que no este dentro de ninguna habitacion supondremos que esta en la misma
	// habitacion que anteriormente
	if(m_actualRoom != nullptr){
		// Calculamos el area de la habitacion
		vector3df firstSide = m_actualRoom->GetFirstSide();
		vector3df secondSide = m_actualRoom->GetSecondSide();
		float perimeter = abs(secondSide.X - firstSide.X) + abs(secondSide.Z - firstSide.Z);
		float value = 1/(perimeter/6.0f) * 100;
		//if(value<10) value = 10.0f;
		// Al encontrarnos en esta habitacion aumentamos su nivel de seguridad
		m_actualRoom->ChangeSecurityLevel(value*deltaTime);

		//std::cout<<m_actualRoom->GetSecurityLevel()<<" "<<perimeter<<" "<<value<<std::endl;
	}
}

void RoomGraph::UpdateExplore(vector3df pos){
	if(m_actualRoom!=nullptr) m_actualRoom->UpdateExplore(pos);
}

vector3df RoomGraph::WhereExplore(vector3df pos){
	vector3df output = 0;
	if(m_actualRoom!=nullptr) output = m_actualRoom->WhereExplore(pos);
	return output;
}

RoomInfo* RoomGraph::GetUnexploredRoom(){
	RoomInfo* output = nullptr;

	ShuffleVector();
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		if(!info->GetExplored()){
			output = info;
			break;
		}
	}
	return output;
}

bool RoomGraph::NextRoom(){
	// En el caso de haber llegado a la habitacion que teniamos planeado
	// reseteamos la variable a nullptr
	if(m_nextRoom!=nullptr){
		if(m_nextRoom == m_actualRoom){
			m_nextRoom = nullptr;
		}
	}
	// Primer intento para saber cual sera la siguiente habitacion
	// Miramos entre las habitaciones contiguas
	if(m_nextRoom == nullptr){
		m_nextRoom = m_actualRoom->GetNextRoom();
	}
	// Segundo intento para saber cual será la siguiente habitacion
	// Miramos si queda alguna habitacion sin explorar
	if(m_nextRoom == nullptr){
		m_nextRoom = GetUnexploredRoom();
	}
	// Se acabaron los intentos, en el caso de que no exista ninguna habitacion
	// Nos rendimos y devolvemos un buen false
	if(m_nextRoom == nullptr){
		return false;
	}
	return true;
}

vector3df RoomGraph::NextRoomPos(){
	vector3df output;
	if(m_nextRoom!=nullptr){
		output = m_nextRoom->GetPosition();
	}
	return output;
}

void RoomGraph::ShuffleVector(){
	int n = m_rooms.size();
	while(n>1){
		int k = rand() % n;
		n--;
		RoomInfo* temp = m_rooms[k];
		m_rooms[k] = m_rooms[n];
		m_rooms[n] = temp;
	}
}

vector3df RoomGraph::GetFirstCorner(){
	vector3df output;
	if(m_actualRoom!=nullptr) output = m_actualRoom->GetFirstSide();
	return output;
}

vector3df RoomGraph::GetSecondCorner(){
	vector3df output;
	if(m_actualRoom!=nullptr) output = m_actualRoom->GetSecondSide();
	return output;
}

vector3df RoomGraph::GetEscapeRoom(vector3df player, vector3df target){
	vector3df output;
	if(m_actualRoom != nullptr) output = m_actualRoom->GetEscapeRoom(player, target);
	return output;
}

bool RoomGraph::IntoSameRoom(vector3df pos){
	bool output = false;
	if(m_actualRoom != nullptr) output = m_actualRoom->IntoSameRoom(pos);
	return output;
}

bool RoomGraph::IntoNextRoom(vector3df pos){
	bool output = false;
	if(m_actualRoom != nullptr) output = m_actualRoom->IntoNextRoom(pos);
	return output;
}
