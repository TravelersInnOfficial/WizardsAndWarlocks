#include "RoomGraph.h"

RoomGraph::RoomGraph(){}

RoomGraph::RoomGraph(RoomGraph* graph){
	graph->CopyGraph(this);
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
