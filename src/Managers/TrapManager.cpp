#include "TrapManager.h"
#include "PlayerManager.h"
#include <vector3d.h>
#include <NetworkEngine/NetworkEngine.h>
#include <GraphicEngine/GraphicEngine.h>

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

TrapManager::TrapManager(){}

TrapManager::~TrapManager(){
	EmptyObject();
}

void TrapManager::InitObject(){
	lastTrapId = 0;
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

	std::map<Player*,std::vector<TFDrawable*>>::iterator it = traps_hud.begin();
	for(;it!=traps_hud.end(); ++it){
		std::vector<TFDrawable*> drawables = it->second;

		for(int i = 0; i<drawables.size();i++) drawables[i]->Erase();
		drawables.clear();

		traps_hud.erase(it);
	}
	traps_hud.clear();
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

void TrapManager::DrawHUD(Player* player){
	//comprobamos si existe el player en el mapa
	std::map<Player*,std::vector<TFDrawable*>>::iterator it = traps_hud.find(player);
	std::vector<TFDrawable*> drawables;

	if(it != traps_hud.end()){
		//si existe comparamos el estado de sus drawables	
		drawables = it->second;

		if(drawables.size() > 0){
			//si la textura asignada es diferente a la actual del jugador se actualiza
			std::string kindImagePath = GetPathFromEnum(getPlayerTrap(player));
			if(drawables[1]->GetTexture() != kindImagePath && kindImagePath.length() > 0){
				drawables[1]->SetTexture(kindImagePath);
			}

			std::string countImagePath = GetPathFromUsings(getPlayerUsings(player));
			if(drawables[2]->GetTexture() != countImagePath && countImagePath.length() > 0){
				drawables[2]->SetTexture(countImagePath);
			}
		}
	}
	//si no existe se añade el player y sus trampas
	else{
		std::string kindImagePath = GetPathFromEnum(getPlayerTrap(player));
		if(kindImagePath.length() > 0){

			std::string countImagePath = GetPathFromUsings(getPlayerUsings(player));
			
			GraphicEngine* g_engine = GraphicEngine::getInstance();
			float W =			g_engine->GetScreenWidth();
			float H =			g_engine->GetScreenHeight();
			float size =		W * 0.075;
			float xInit =		W * 0.87;
			float yInit =		H - size*3.5;
			float outline =		5;

			TFRect* m_bckg = toe::Add2DRect(toe::core::TOEvector2df(xInit,yInit), toe::core::TOEvector2df(size+outline,size+outline));
			drawables.push_back(m_bckg);

			vector4df sizeImage(xInit + outline, yInit + outline, xInit + size - outline, yInit + size - outline);
			TFSprite* m_trap_sprite = toe::AddSprite(kindImagePath, toe::core::TOEvector2df(xInit + outline, yInit + outline), toe::core::TOEvector2df(size-outline, size-outline));
			drawables.push_back(m_trap_sprite);
		
			vector4df sizeCounter(xInit + size, sizeImage.Y, xInit + size + size/2, sizeImage.Y2);
			TFSprite* m_trap_count = toe::AddSprite(countImagePath, toe::core::TOEvector2df(xInit + size*0.75, yInit - size/4), toe::core::TOEvector2df(size/2,size/2));
			drawables.push_back(m_trap_count);

			traps_hud.insert(std::pair<Player*,std::vector<TFDrawable*>>(player,drawables));
	
		}
	}
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