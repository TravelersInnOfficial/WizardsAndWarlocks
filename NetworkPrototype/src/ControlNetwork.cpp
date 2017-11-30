#include "ControlNetwork.h"

ControlNetwork* ControlNetwork::instance = 0;

ControlNetwork* ControlNetwork::GetInstance(){
	if(instance == 0) instance = new ControlNetwork();
	return instance;
}

ControlNetwork::ControlNetwork(){

}

ControlNetwork::~ControlNetwork(){

}

void ControlNetwork::Update(){
	RetrieveObjects();
	SpawnNewObjects();
}

void ControlNetwork::RetrieveObjects(){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();

	if(n_engine->IsServerInit()){
		networkObjects = n_engine->GetServer()->GetNetworkObjects();
		newNetworkObjects = n_engine->GetServer()->GetNewNetworkObjects();
	}

	else if(n_engine->IsClientInit()){
		networkObjects = n_engine->GetClient()->GetNetworkObjects();
		newNetworkObjects = n_engine->GetClient()->GetNewNetworkObjects();
	}
}

void ControlNetwork::SpawnNewObjects(){
	for (auto &row : newNetworkObjects) {
		std::cout<<"ENTRAMOS EN EL BUCLE"<<std::endl;
		switch(row.second->GetObjType()){
			case ID_NO_OBJ:{
				continue;
				break;
			}
			case ID_PLAYER_O:{
				std::cout<<"SPAWNEAR JUGADOR!!!!"<<std::endl;
				break;
			}
			case ID_POTION_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
			case ID_FOUNTAIN_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
			case ID_DOOR_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
			case ID_SWITCH_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
			case ID_GRAIL_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
			case ID_TRAP_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
			case ID_PROYECTIL_O:{
				std::cout<<"SPAWNEAR OTRO OBJ!!!!"<<std::endl;
				break;
			}
		}
	}
	std::map<int, NetworkObject*> emptyMap;
	newNetworkObjects = emptyMap;
}