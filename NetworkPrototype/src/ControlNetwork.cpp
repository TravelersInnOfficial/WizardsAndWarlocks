#include "ControlNetwork.h"
#include "./Game.h"

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
	EraseObjects();
}

void ControlNetwork::RetrieveObjects(){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();

	if(n_engine->IsServerInit()){
		networkObjects = n_engine->GetServer()->GetNetworkObjects();
		newNetworkObjects = n_engine->GetServer()->GetNewNetworkObjects();
		toEraseNetworkObjects = n_engine->GetServer()->GetToEraseNetworkObjects();
	}

	else if(n_engine->IsClientInit()){
		networkObjects = n_engine->GetClient()->GetNetworkObjects();
		newNetworkObjects = n_engine->GetClient()->GetNewNetworkObjects();
		toEraseNetworkObjects = n_engine->GetClient()->GetToEraseNetworkObjects();
	}
}

void ControlNetwork::SpawnNewObjects(){
	  std::map<int, NetworkObject*>::reverse_iterator row;
	  for (row = newNetworkObjects.rbegin(); row != newNetworkObjects.rend(); row++){
		switch(row->second->GetObjType()){
			case ID_NO_OBJ:{
				continue;
				break;
			}
			case ID_PLAYER_O:{
				Game* gameInstance = Game::GetInstance();
				gameInstance->SetPlayerOne(row->second);
				break;
			}
			case ID_POTION_O:{
				break;
			}
			case ID_FOUNTAIN_O:{
				break;
			}
			case ID_DOOR_O:{
				break;
			}
			case ID_SWITCH_O:{
				break;
			}
			case ID_GRAIL_O:{
				break;
			}
			case ID_TRAP_O:{
				break;
			}
			case ID_PROYECTIL_O:{
				break;
			}
		}
	}
	std::map<int, NetworkObject*> emptyMap;
	newNetworkObjects = emptyMap;
}

void ControlNetwork::EraseObjects(){
	std::map<int, NetworkObject*>::reverse_iterator row;
	for (row = toEraseNetworkObjects.rbegin(); row != toEraseNetworkObjects.rend(); row++){
		if(row->second == NULL) continue;
		switch(row->second->GetObjType()){
			case ID_NO_OBJ:{
				continue;
				break;
			}
			case ID_PLAYER_O:{
				std::cout<<"RECIBIMOS MENSAJE BORRAR"<<std::endl;
				ControlPlayer::GetInstance()->AddToDeletePlayer(row->second->GetObjId());
				break;
			}
			case ID_POTION_O:{
				break;
			}
			case ID_FOUNTAIN_O:{
				break;
			}
			case ID_DOOR_O:{
				break;
			}
			case ID_SWITCH_O:{
				break;
			}
			case ID_GRAIL_O:{
				break;
			}
			case ID_TRAP_O:{
				break;
			}
			case ID_PROYECTIL_O:{
				break;
			}
		}
	}
	toEraseNetworkObjects.clear();
}