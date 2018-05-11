#include "TrapManager.h"

#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/NetworkObject.h>
#include <NetworkEngine/Client.h>
#include <NetworkEngine/Server.h>
#include <PhysicsEngine/BulletEngine.h>
#include <GraphicEngine/GSprite.h>
#include <GraphicEngine/GRect.h>
#include "./../Objects/Trap.h"
#include "./../Players/Player.h"
#include "PlayerManager.h"
#include <fstream>
#include <json.hpp>

static TrapManager* instance = nullptr;

TrapManager* TrapManager::GetInstance(){
	static TrapManager localInstance;
	if(instance == nullptr){
		localInstance.InitObject();
		instance = &localInstance;
	}
	return instance;
}

void TrapManager::Update(float deltaTime){
	UpdateTrap(deltaTime);
}

TrapManager::TrapManager(){
	trapHud = nullptr;
	numberTrap = nullptr;
	rectHud = nullptr;
}

TrapManager::~TrapManager(){
	EmptyObject();
}

void TrapManager::InitObject(){
	lastTrapId = 0;
	loadJSONTrapData();
}

void TrapManager::loadJSONTrapData(){
	//Takes path from binary location (/bin)
	std::string jsonPath = "./../assets/json/game_data.json";
	std::ifstream i(jsonPath);
	nlohmann::json j;
	i >> j;

	std::map<std::string, TrapEnum> codesMap = GetTRAPENUM_StrMap();
	TrapEnum ID;

	for(int i = 0; !j["traps_data"][i].is_null(); i++){
		ID = codesMap[ j["traps_data"][i]["ID"] ];

		trap_EFFECT.insert(std::pair<TrapEnum,std::string>(ID, j["traps_data"][i]["EFFECT_ID"]));
		trap_name.insert(std::pair<TrapEnum,std::string>(ID, j["traps_data"][i]["name"]));
		trap_description.insert(std::pair<TrapEnum,std::string>(ID, j["traps_data"][i]["description"]));
		trap_damage.insert(std::pair<TrapEnum,float>(ID, j["traps_data"][i]["damage"]));
	}

}

void TrapManager::EmptyObject(){
	ClearTraps();
	playerTrap.clear();
	playerUsings.clear();
	instance = nullptr;
}

void TrapManager::ClearTraps(){
	int size = traps.size();
	for(int i=0; i<size; i++){
		Trap* t = traps[i];
		delete t;
	}
	traps.clear();

	if(trapHud != nullptr) delete trapHud;
	if(numberTrap != nullptr) delete numberTrap;
	if(rectHud != nullptr) delete rectHud;

	trapHud = nullptr;
	numberTrap = nullptr;
	rectHud = nullptr;
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

	Alliance alliance = NO_ALLIANCE;
	if(player != nullptr) alliance = player->GetAlliance();

	if(uses > 0 && alliance == ALLIANCE_WARLOCK){
		if(DeployTrap(getPlayerTrap(player),Start,End, player->GetId())){
			uses--;
			setPlayerUsings(player,uses);
			toRet = true;
		}
	}

	return toRet;
}

bool TrapManager::DeployTrap(TrapEnum type,vector3df Start, vector3df End, int playerId){
	bool toRet = false;
	vector3df point(0,0,0);
	vector3df normal(0,0,0);
	BulletEngine::GetInstance()->Raycast(Start, End, &point, &normal);

	// Paredes
	Trap* myTrap = AddTrap(point,normal,type);
	toRet = true;
	
	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(n_engine->IsServerInit()){
		Server* server = n_engine->GetServer();
		if(server != nullptr){
			myTrap->SetTrapId(lastTrapId);
			Player* player = PlayerManager::GetInstance()->GetPlayerFromID(playerId);
			int netPlayerId = player->GetNetworkObject()->GetObjId();
			server->SetTrap(point, normal, netPlayerId, lastTrapId++);
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
		Player* p = it->first;
		if(p != nullptr && p == player) return it->second;
	}

	return -1;
}

bool TrapManager::setPlayerTrap(Player* player, TrapEnum trap, bool broadcast){
	bool toRet = false;
	std::map<Player*, TrapEnum>::iterator it = playerTrap.begin();
	
	for(; it != playerTrap.end(); ++it){
		if(it->first == player){
			it->second = trap;
			toRet = true;
		}
	}

	if(broadcast){
		// Si somos cliente y player one, sincronizarlo
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		if(player->IsPlayerOne() && n_engine->IsClientInit()){
			Client* client = n_engine->GetClient();
			if(client != nullptr){
				int netPlayerId = player->GetNetworkObject()->GetObjId();
				client->SetPlayerTrap(netPlayerId, trap);
			}
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
    Trap* myTrap = nullptr;
    
    if(player != nullptr){
        int uses = getPlayerUsings(player);
		uses--;
        setPlayerUsings(player, uses);
        type = getPlayerTrap(player);
    }

    if(type != TENUM_NO_TRAP) myTrap = AddTrap(position, normal, type);
    if(myTrap != nullptr) myTrap->SetTrapId(id);
}

// Only for NETWORK
void TrapManager::NoPlayerDeploy(vector3df position, vector3df normal, TrapEnum type, int id){
	Trap* myTrap = AddTrap(position, normal, type);
	if(myTrap != nullptr) myTrap->SetTrapId(id);
}

void TrapManager::IdErase(int id){
	Trap* trapToErase = GetTrapWithId(id);
	if(trapToErase != nullptr) DeleteTrap(trapToErase);
}

Trap* TrapManager::GetTrapWithId(int id){
	Trap* toRet = nullptr;

	int size = traps.size();
	for(int i = size-1; i >= 0; i--){
		Trap* t = traps[i];
		if(t != nullptr && t->GetTrapId() == id) toRet = t;
	}

	return toRet;
}

std::vector<Trap*> TrapManager::GetAllTraps(){
	return(traps);
}

std::string TrapManager::GetPathFromEnum(TrapEnum tKind){
	std::string toRet = "./../assets/textures/HUD/Traps/";
	
	switch(tKind){
        case TENUM_DEATH_CLAWS:
			toRet += "TENUM_DEATH_CLAWS.png";
        break;

        case TENUM_SPIRITS:
			toRet += "TENUM_SPIRITS.png";
        break;

        case TENUM_SILENCE:
			toRet += "TENUM_SILENCE.png";
        break;

        case TENUM_TAXES:
			toRet += "TENUM_TAXES.png";
        break;
        
        case TENUM_DISTURBANCE:
			toRet += "TENUM_DISTURBANCE.png";
        break;

        case TENUM_EXPLOSIVE:
			toRet += "TENUM_EXPLOSIVE.png";
        break;

        default:
			toRet = "";
        break;
    }

	return toRet;
}

std::string TrapManager::GetPathFromUsings(int usings){
	if(usings < 0) usings = 0;
	else if(usings > MaxUsings) usings = MaxUsings;
	std::string toRet = "./../assets/textures/HUD/Counter/" + std::to_string(usings) + ".png";
	return toRet;
}

void TrapManager::ErasePlayer(Player* player){
	playerTrap.erase(player);
	playerUsings.erase(player);
}

// For refreshing newcomers on the server
void TrapManager::RefreshServerAll(){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(n_engine->IsServerInit()){
		Server* server = n_engine->GetServer();
		if(server != nullptr){
			for (std::map<Player*, TrapEnum>::iterator it=playerTrap.begin(); it!=playerTrap.end(); ++it){
				Player* p = it->first;
				if(p != nullptr){
					NetworkObject* nObj = p->GetNetworkObject();
					if(nObj != nullptr){
						int netPlayerId = nObj->GetObjId();
						server->SetPlayerTrap(netPlayerId, it->second, getPlayerUsings(p));
					}
				}
			}
		}
	}
}

std::string TrapManager::GetTrapEffect(TrapEnum trap){
	return trap_EFFECT[trap];
}

std::string TrapManager::GetTrapName(TrapEnum trap){
	return trap_name[trap];
}

std::string TrapManager::GetTrapDescription(TrapEnum trap){
	return trap_description[trap];
}

float TrapManager::GetTrapDamage(TrapEnum trap){
	return trap_damage[trap];
}


std::map<std::string, TrapEnum> TrapManager::GetTRAPENUM_StrMap(){
	std::map<std::string, TrapEnum> ret_map;
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_NO_TRAP", TENUM_NO_TRAP));
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_DEATH_CLAWS", TENUM_DEATH_CLAWS));
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_SPIRITS", TENUM_SPIRITS));
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_SILENCE", TENUM_SILENCE));
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_TAXES", TENUM_TAXES));
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_DISTURBANCE", TENUM_DISTURBANCE));
	ret_map.insert(std::pair<std::string, TrapEnum>("TENUM_EXPLOSIVE", TENUM_EXPLOSIVE));

	return ret_map;
};