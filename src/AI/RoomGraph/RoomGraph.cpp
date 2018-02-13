#include "RoomGraph.h"

#include <limits>

RoomGraph::RoomGraph(){
	m_actualRoom = NULL;
	m_nextRoom = NULL;
}

RoomGraph::RoomGraph(RoomGraph* graph){
	graph->CopyGraph(this);
	m_actualRoom = NULL;
	m_nextRoom = NULL;
}

RoomGraph::~RoomGraph(){
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		delete info;
	}
	m_rooms.clear();
}

RoomInfo* RoomGraph::AddRoom(int id, vector3df position, vector3df firstSide, vector3df secondSide){
	RoomInfo* room = new RoomInfo(id, position, firstSide, secondSide);
	m_rooms.push_back(room);
	return room;
}

bool RoomGraph::AddConnection(int first, int second){
	RoomInfo* firstRoom = NULL;
	RoomInfo* secondRoom = NULL;
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
		if(firstRoom!=NULL && secondRoom!=NULL){
			break;
		}
	}
	// Comprobamos si hemos acabado el bucle con los dos valores o sin alguno de ellos
	if(firstRoom!=NULL && secondRoom!=NULL){
		firstRoom->AddNextRoom(secondRoom);
		return true;
	}
	// No se anyade la conexion, puesto que no se han encontrado los elementos
	return false;
}

bool RoomGraph::RoomExplored(){
	bool output = false;
	if(m_actualRoom!=NULL) output = m_actualRoom->GetExplored();
	return output;
}

vector3df RoomGraph::RoomPos(){
	vector3df output;
	if(m_actualRoom!=NULL) output = m_actualRoom->GetPosition();
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
	float output = false;
	if((C<A && C>B) || (C>A && C<B)){
		output = true;
	}
	return output;
}

void RoomGraph::InitRoom(vector3df pos){
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		vector3df firstSide = info->GetFirstSide();
		vector3df secondSide = info->GetSecondSide();
		if(CheckInside(firstSide.X, secondSide.X, pos.X) && CheckInside(firstSide.Z, secondSide.Z, pos.Z)){
			m_actualRoom = m_rooms[i];
			break;	
		}
	}
}

void RoomGraph::UpdateExplore(vector3df pos){
	if(m_actualRoom!=NULL) m_actualRoom->UpdateExplore(pos);
}

vector3df RoomGraph::WhereExplore(vector3df pos){
	vector3df output = 0;
	if(m_actualRoom!=NULL) output = m_actualRoom->WhereExplore(pos);
	return output;
}

RoomInfo* RoomGraph::GetUnexploredRoom(){
	RoomInfo* output = NULL;

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
	// reseteamos la variable a NULL
	if(m_nextRoom!=NULL){
		if(m_nextRoom == m_actualRoom){
			m_nextRoom = NULL;
		}
	}
	// Primer intento para saber cual sera la siguiente habitacion
	// Miramos entre las habitaciones contiguas
	if(m_nextRoom == NULL){
		m_nextRoom = m_actualRoom->GetNextRoom();
	}
	// Segundo intento para saber cual será la siguiente habitacion
	// Miramos si queda alguna habitacion sin explorar
	if(m_nextRoom == NULL){
		m_nextRoom = GetUnexploredRoom();
	}
	// Se acabaron los intentos, en el caso de que no exista ninguna habitacion
	// Nos rendimos y devolvemos un buen false
	if(m_nextRoom == NULL){
		return false;
	}
	return true;
}

vector3df RoomGraph::NextRoomPos(){
	vector3df output;
	if(m_nextRoom!=NULL){
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