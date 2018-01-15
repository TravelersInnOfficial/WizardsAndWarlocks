#include "TrapManager.h"
#include "PlayerManager.h"
#include <vector3d.h>
#include "./../NetworkEngine/NetworkEngine.h"

TrapManager* TrapManager::instance = 0;

TrapManager* TrapManager::GetInstance(){
	if(instance == 0) instance = new TrapManager();
	return instance;
}

void TrapManager::Update(float deltaTime){
	UpdateTrap(deltaTime);
}

TrapManager::TrapManager(){
	lastTrapId = 0;
}

TrapManager::~TrapManager(){
	ClearTraps();
	playerTrap.clear();
	playerUsings.clear();
}

void TrapManager::ClearTraps(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();
}

Trap* TrapManager::AddTrap(vector3df pos, vector3df normal, TrapEnum type){
	Trap* t = new Trap(pos,normal, type);
	traps.push_back(t);
	return t;
}

void TrapManager::AddTrapToPlayer(Player* player, TrapEnum trap){
	TrapEnum actualTrap = getPlayerTrap(player);

	// Si el jugador ya tiene una trampa asignada se cambia por una nueva
	if(actualTrap != TENUM_NO_TRAP){
		setPlayerTrap(player,trap);
		setPlayerUsings(player,MaxUsings);
	}

	// Si no la tiene se la asignamos
	else{
		playerTrap.insert(std::pair<Player*, TrapEnum>(player,trap));
		playerUsings.insert(std::pair<Player*, int>(player,MaxUsings));
	}
}

bool TrapManager::PlayerDeployTrap(Player* player,vector3df Start, vector3df End){
	bool toRet = false;
	
	int uses = getPlayerUsings(player);
	if(uses == 0) return false;
	if(DeployTrap(getPlayerTrap(player),Start,End, player->GetId())){
		uses--;
		setPlayerUsings(player,uses);
		toRet = true;
	}

	return toRet;
}

bool TrapManager::DeployTrap(TrapEnum type,vector3df Start, vector3df End, int playerId){
	bool toRet = false;
	vector3df point(0,0,0);
	vector3df normal(0,0,0);
	GraphicEngine::getInstance()->Raycast(Start,End, &point, &normal);

	// Paredes
	if(!(normal.X == 0 && normal.Y != 0 && normal.Z == 0)
	&& !(normal.X == 0 && normal.Y == 0 && normal.Z == 0)
	&& !(normal.X == 90 && normal.Y == 0 && normal.Z == 0)
	&& (normal.Y >=0 && normal.Y <=180)
	){
		Trap* myTrap = AddTrap(point,normal,type);
		toRet = true;
		
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		if(n_engine->IsServerInit()){
			Server* server = n_engine->GetServer();
			if(server != NULL){
				myTrap->SetTrapId(lastTrapId);
				Player* player = PlayerManager::GetInstance()->GetPlayerFromID(playerId);
				int netPlayerId = player->GetNetworkObject()->GetObjId();
				server->SetTrap(point, normal, netPlayerId, lastTrapId++);
			}
		}
	}

	return toRet;
}

void TrapManager::SendAllSignal(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		t->SendSignal();
	}
}

void TrapManager::DeleteTrap(Trap* trap){
	int size = traps.size();
	for(int i=size-1; i>=0; i--){
		Trap* t = traps[i];
		if(t == trap){
			traps.erase(traps.begin() + i);
			delete t;
		}
	}
}

void TrapManager::UpdateTrap(float deltaTime){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		t->Update(deltaTime);
	}
}

TrapEnum TrapManager::getPlayerTrap(Player* player){
	std::map<Player*, TrapEnum>::iterator it = playerTrap.begin();
	
	for(; it != playerTrap.end(); ++it){
		if(it->first == player) return it->second;
	}

	return TENUM_NO_TRAP;
}

int TrapManager::getPlayerUsings(Player* player){
	std::map<Player*, int>::iterator it = playerUsings.begin();
	
	for(; it != playerUsings.end(); ++it){
		if(it->first == player) return it->second;
	}

	return -1;
}

bool TrapManager::setPlayerTrap(Player* player, TrapEnum trap){
	bool toRet = false;
	std::map<Player*, TrapEnum>::iterator it = playerTrap.begin();
	
	for(; it != playerTrap.end(); ++it){
		if(it->first == player){
			it->second = trap;
			toRet = true;
		}
	}

	return toRet;
}
bool TrapManager::setPlayerUsings(Player* player, int uses){
	bool toRet = false;
	std::map<Player*, int>::iterator it = playerUsings.begin();
	if(uses > MaxUsings) uses = MaxUsings;
	
	for(; it != playerUsings.end(); ++it){
		if(it->first == player){
			it->second = uses;
			toRet = true;
		}
	}

	return toRet;
}

// Only for NETWORK
void TrapManager::DirectDeploy(int playerId, vector3df position, vector3df normal, int id){
	Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerId);
    TrapEnum type = TENUM_NO_TRAP;
    Trap* myTrap = NULL;
    
    if(player != NULL){
        int uses = getPlayerUsings(player);
        setPlayerUsings(player, uses--);
        type = getPlayerTrap(player);
    }

    if(type != TENUM_NO_TRAP) myTrap = AddTrap(position, normal, type);
    if(myTrap != NULL) myTrap->SetTrapId(id);
}

void TrapManager::IdErase(int id){
	Trap* trapToErase = GetTrapWithId(id);
	if(trapToErase != NULL) DeleteTrap(trapToErase);
}

Trap* TrapManager::GetTrapWithId(int id){
	Trap* toRet = NULL;

	int size = traps.size();
	for(int i = size-1; i >= 0; i--){
		Trap* t = traps[i];
		if(t != NULL && t->GetTrapId() == id) toRet = t;
	}

	return toRet;
}