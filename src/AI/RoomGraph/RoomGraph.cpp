#include "RoomGraph.h"

#include <limits>

RoomGraph::RoomGraph(){
	m_actualRoom = NULL;
}

RoomGraph::RoomGraph(RoomGraph* graph){
	graph->CopyGraph(this);
	m_actualRoom = NULL;
}

RoomGraph::~RoomGraph(){
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		delete info;
	}
	m_rooms.clear();
}

RoomInfo* RoomGraph::AddRoom(int id, vector3df position){
	RoomInfo* room = new RoomInfo(id, position);
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

void RoomGraph::CopyGraph(RoomGraph* copyGraph){
	// Copiar todas las habitaciones
	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		copyGraph->AddRoom(info->GetId(), info->GetPosition());
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

void RoomGraph::InitRoom(vector3df pos){

    float compare = std::numeric_limits<float>::max();
    RoomInfo* nearest = NULL;

	int size = m_rooms.size();
	for(int i=0; i<size; i++){
		RoomInfo* info = m_rooms[i];
		vector3df difPos = pos - info->GetPosition();
		float value = difPos.length();	// Sacamos la distancia entre la posicion y la habitacion
		if(value<compare){
			compare = value;
			nearest = info;
		}
	}

	if(m_actualRoom != nearest){
		if(m_actualRoom != NULL){
			// Cambios a hacer al cambiar de habitacion
			// Tipo el security level
		}
		m_actualRoom = nearest;
	}
}

void RoomGraph::UpdateExplore(float rotY){
	if(m_actualRoom!=NULL) m_actualRoom->UpdateExplore(rotY);
}

