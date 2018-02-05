#include "NetworkManager.h"

NetworkManager* NetworkManager::instance = 0;

NetworkManager* NetworkManager::GetInstance(){
	if(instance == 0) instance = new NetworkManager();
	return instance;
}

NetworkManager::NetworkManager(){ 
	multiGame = NULL;
}

NetworkManager::~NetworkManager(){
	instance = 0;
}

NetworkObject* NetworkManager::GetMultiGame(){
	return multiGame;
}

void NetworkManager::Update(){
	RetrieveObjects();
	SpawnNewObjects();
	EraseObjects();
}

void NetworkManager::RetrieveObjects(){
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

void NetworkManager::SpawnNewObjects(){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();

	std::map<int, NetworkObject*>::reverse_iterator row;
	for (row = newNetworkObjects.rbegin(); row != newNetworkObjects.rend(); row++){
		switch(row->second->GetObjType()){
			case ID_NO_OBJ:{
				continue;
				break;
			}
			case ID_PLAYER_O:{
				//NetGame* gameInstance = NetGame::GetInstance();
				bool playerOne = false;
				std::string name = "";

				// Comprobar si es PLAYER ONE con la ID del NETWORK OBJECT y mi ID del OBJETO PLAYER ONE
				if(n_engine->IsClientInit() && row->second->GetObjId() == n_engine->GetClient()->GetPlayerOneId()){
					playerOne = true;
					name = n_engine->GetClient()->GetClientName();
				}

				PlayerManager* playerManager = PlayerManager::GetInstance();
				Player* player = playerManager->AddHumanPlayer(playerOne);
				player->SetNetworkObject(row->second);
				if(name.length() > 0) player->SetName(name);

				//gameInstance->CreatePlayer(row->second, playerOne, name);
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
			case ID_MULTIGAME_O:{
				multiGame = row->second;
				break;
			}
		}
	}
	std::map<int, NetworkObject*> emptyMap;
	newNetworkObjects = emptyMap;
}

void NetworkManager::EraseObjects(){
	std::map<int, NetworkObject*>::reverse_iterator row;
	for (row = toEraseNetworkObjects.rbegin(); row != toEraseNetworkObjects.rend(); row++){
		if(row->second == NULL) continue;
		switch(row->second->GetObjType()){
			case ID_NO_OBJ:{
				continue;
				break;
			}
			case ID_PLAYER_O:{
				PlayerManager::GetInstance()->AddToDeletePlayer(row->second->GetObjId());
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
			case ID_MULTIGAME_O:{
				break;
			}
		}
	}
	toEraseNetworkObjects.clear();
}