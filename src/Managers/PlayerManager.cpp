#include "PlayerManager.h"
#include "ObjectManager.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::GetInstance(){
	if(instance == nullptr) instance = new PlayerManager();
	return instance;
}

PlayerManager::PlayerManager(){
	wizardsWin = false;
	warlocksWin = false;
	playerOne = nullptr;
}

PlayerManager::~PlayerManager(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		delete p;
	}
	players.clear();

	size = deadPlayers.size();
	for(int i=0; i<size; i++){
		Player* p = deadPlayers[i];
		delete p;
	}
	deadPlayers.clear();

	instance = nullptr;
	wizardsWin = false;
	warlocksWin = false;
	playerOne = nullptr;
}

Player* PlayerManager::AddHumanPlayer(bool isPlayer1){
	Player* p = new HumanPlayer(isPlayer1);
	// En el caso de que sea el jugador 1 no slo guardamos
	if(isPlayer1){
		playerOne = p;
	}
	players.push_back(p);
	p->SetAlliance(ALLIANCE_WARLOCK);
	return p;
}

AIPlayer* PlayerManager::AddAIPlayer(){
	AIPlayer* p = new AIPlayer();
	players.push_back(p);
	p->SetAlliance(ALLIANCE_WIZARD);
	return p;
}

void PlayerManager::UpdatePlayers(float deltaTime, bool isNetGame){
	DeletePlayers();
	// Actualizamos los personajes vivos
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];

		if(isNetGame) {
			HumanPlayer* hp = (HumanPlayer*) players[i];
			NetworkEngine* n_engine = NetworkEngine::GetInstance();
			bool isServer = n_engine->IsServerInit();
			if(p->IsPlayerOne() || isServer) hp->SetNetInput();
			hp->GetNetInput();
		}

		p->Update(deltaTime);		
	}
	
	// Actualizamos los personajes muertos (camara libre)
	size = deadPlayers.size();
	for(int i=0; i<size; i++){
		Player* p = deadPlayers[i];
		if(isNetGame) {
			HumanPlayer* hp = (HumanPlayer*) deadPlayers[i];
			if(p->IsPlayerOne()) hp->SetNetInput();
			else hp->GetNetInput();
		}
		p->DeadUpdate();
	}
}

void PlayerManager::AddToDeletePlayer(int networkId){
	int size = players.size();
	for(int i=size-1; i>=0; i--){
		Player* p = players[i];
		if(p->GetNetworkObject()->GetObjId() == networkId){
			players.erase(players.begin()+i);
			playersToDelete.push_back(p);
			break;
		}
	}
}

void PlayerManager::DeletePlayers(){
	int size = playersToDelete.size();
	for(int i=0; i<size; i++){
		Player* p = playersToDelete[i];
		delete p;
	}
	playersToDelete.clear();
}

void PlayerManager::SendVisualSignal(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->SendSignal();
	}
}

void PlayerManager::ResetAllSpells(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->ResetAllSpells();
	}
}

// Comprobamos TODOS las variables READY (Solo para RED)
bool PlayerManager::CheckIfReady(){
	bool allReady = true;
	bool theresWizard = false;
	bool theresWarlock = false;
	int size = players.size();
	
	// Si alguno de los personajes no esta READY
	// Devolvemos una FALSE
	for(int i=0; i < size && allReady == true; i++){
		Player* p = players[i];
		
		if(p->GetAlliance() == ALLIANCE_WARLOCK) theresWarlock = true;
		else if(p->GetAlliance() == ALLIANCE_WIZARD) theresWizard = true;
		
		if(!p->CheckIfReady()) allReady = false;
	}

	if(allReady && (size < 2 || !theresWizard || !theresWizard)) allReady = false;

	return allReady;
}

// Revisar inicio de partida, al hacer respawn se intenta volverlos a la vida otra vez
void PlayerManager::RespawnDeadPlayers(){
	int size = deadPlayers.size();
	for(int i=size-1; i>=0; i--){
		Player* p = deadPlayers[i];
		if(p != nullptr){
			deadPlayers.erase(deadPlayers.begin() + i);
			players.push_back(p);
			p->Respawn();
		}
	}
}

void PlayerManager::RestartMatchStatus(){
	// Pasamos todos los jugadores muertos a vivos
	int size = deadPlayers.size();
	for(int i=size-1; i>=0; i--){
		Player* p = deadPlayers[i];
		deadPlayers.erase(deadPlayers.begin() + i);	
		players.push_back(p);
	}

	size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->RestartMatchStatus();
	}

	warlocksWin = false;
	wizardsWin = false;
}

void PlayerManager::InitGame(){
	// Pasamos al vector de vivos a todos los jugadores
	int size = deadPlayers.size();
	for(int i=size-1; i>=0; i--){
		Player* p = deadPlayers[i];
		players.push_back(p);
	}
	deadPlayers.clear();
	// Inicializamos la partida de todos los jugadores
	size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->InitGame();
	}
	// Cambiamos las variables de partida
	warlocksWin = false;
	wizardsWin = false;
}

void PlayerManager::ManageMatchStatus(bool started){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->SetMatchStatus(started);
	}
	size = deadPlayers.size();
	for(int i=0; i<size; i++){
		Player* p = deadPlayers[i];
		p->SetMatchStatus(started);
	}
}

void PlayerManager::AddToDead(Player* player){
	// Prefiero hacerlo a mano y asi comprobar si existe o no el player
	//players.erase(std::remove(players.begin(), players.end(), player), players.end());
	//deadPlayers.push_back(player);
	
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(p == player){		// Compruebo si el jugador realmente esta vivo
			// En el caso de estarlo le paso del vector de Players al de DeadPlayers
			players.erase(players.begin() + i);	
			deadPlayers.push_back(player);
			break;
		}
	}

	CheckWon();
}

void PlayerManager::AddToLife(Player* player){
	int size = deadPlayers.size();
	for(int i=0; i<size; i++){
		Player* p = deadPlayers[i];
		if(p == player){		// Compruebo si el jugador realmente esta muerto
			// En el caso de estarlo le paso del vector de deadPlayers al de Players
			deadPlayers.erase(deadPlayers.begin() + i);	
			players.push_back(player);
			break;
		}
	}
}

void PlayerManager::CheckWon(){
	// Ponemos un contador para cada faccion
	int contWiz = 0;
	int contWar = 0;

	// Comprobamos cuantos jugadores de cada faccion quedan vivos
	int size = players.size();
	for(int i=0; i < size; i++){
		Player* p = players[i];
		Alliance alli = p->GetAlliance();
		if(alli == ALLIANCE_WIZARD) contWiz++;
		else if(alli == ALLIANCE_WARLOCK) contWar++;
	}

	// En el caso de que alguno quede a 0 significara la victoria del equipo contrario
	if(contWiz == 0) warlocksWin = true;
	else if(contWar == 0) wizardsWin = true;

}

bool PlayerManager::CheckIfWon(Alliance alliance){
	bool toRet = false;
	if(alliance == ALLIANCE_WIZARD) toRet = wizardsWin;
	else toRet = warlocksWin;

	if(toRet){
		warlocksWin = false;
		wizardsWin = false;
	}

	return toRet;
}

void PlayerManager::EraseAllCharacters(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->DestroyPlayerCharacter();
	}
}

void PlayerManager::ReturnAllToLobby(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->ReturnToLobby();
	}
}

void PlayerManager::RefreshServerAll(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->RefreshServer();
	}
}

Player* PlayerManager::GetPlayerOne(){
	return playerOne;
}

std::vector<Player*> PlayerManager::GetAllPlayers(){
	std::vector<Player*> toRet;
	toRet.insert(toRet.end(), players.begin(), players.end() );
	toRet.insert(toRet.end(), deadPlayers.begin(), deadPlayers.end() );
	return(toRet);
}

Player* PlayerManager::GetPlayerFromID(int id){
	Player* toRet = nullptr;
	
	int size = players.size();
	for(int i=0; i<size && toRet == nullptr; i++){
		Player* p = players[i];
		if(p->GetId() == id) toRet = p;
	}

	return toRet;
}

Player* PlayerManager::GetPlayerFromNetID(int id){
	Player* toRet = nullptr;
	
	int size = players.size();
	for(int i=0; i<size && toRet == nullptr; i++){
		Player* p = players[i];
		NetworkObject* nObj = nullptr;

		if(p != nullptr) nObj = p->GetNetworkObject();
		if(nObj != nullptr && nObj->GetObjId() == id) toRet = p;
	}

	return toRet;
}

bool PlayerManager::PlayerAlive(Player* player){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(p == player){
			return true;
		}
	}
	return false;
}	

Player* PlayerManager::ChangePlayerTargetCam(Player* player, Alliance alli){
	int size = players.size();
	if(size==0) return nullptr; // No hay jugadores vivos

	int value = -1;
	int secondValue = -1;
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(p == player){
			value = i;			// Nos guardamos la posicion del jugador actual
		}

		if(secondValue == -1 && p->GetAlliance()==alli){
			secondValue = i;	// Nos guardamos la posicion del primer jugador del vector con la alianza correcta
		}
		else if(value != -1 && i>value && p->GetAlliance()==alli){
			secondValue = i;	// Comparamos con la posicion del player actual para quedarnos con justo el siguiente
			break;
		}
	}

	// En el caso de que value == -1 siguiendo la formula se pondra el primer jugador del vector
	if(secondValue!=-1){
		return players[secondValue];
	}else{
		return nullptr;
	}
}