#include "PlayerManager.h"
#include "ObjectManager.h"

PlayerManager* PlayerManager::instance = 0;

PlayerManager* PlayerManager::GetInstance(){
	if(instance==0) instance = new PlayerManager();
	return instance;
}

PlayerManager::PlayerManager(){
	wizardsWin = false;
	warlocksWin = false;
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
	instance = 0;
}

Player* PlayerManager::AddHumanPlayer(bool isPlayer1){
	Player* p = new HumanPlayer(isPlayer1);
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

void PlayerManager::UpdatePlayers(bool isNetGame){
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

		p->Update();		
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
		p->Update();
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
		
		if(!p->GetReadyStatus()) allReady = false;
	}

	if(allReady){
		if(size < 2 || !theresWizard || !theresWizard) allReady = false;
	}

	return allReady;
}

void PlayerManager::RespawnDeadPlayers(){
	int size = deadPlayers.size();
	for(int i=0; i<size; i++){
		Player* p = deadPlayers[i];
		if(p != NULL){
			players.push_back(p);
			p->Respawn();
		}
	}
	deadPlayers.clear();
}

void PlayerManager::RestartMatchStatus(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(p != NULL) p->Respawn();
	}
	RespawnDeadPlayers();
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

void PlayerManager::AddToDead(Alliance alliance, Player* player){
	players.erase(std::remove(players.begin(), players.end(), player), players.end());
	deadPlayers.push_back(player);
	
	CheckWon();
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
	Player* toRet = NULL;

	int size = players.size();
	for(int i=0; i<size && toRet == NULL; i++){
		Player* p = players[i];
		if(p->IsPlayerOne()) toRet = p;
	}

	return(toRet);
}

vector<Player*> PlayerManager::GetAllPlayers(){
	vector<Player*> toRet;
	toRet.insert(toRet.end(), players.begin(), players.end() );
	toRet.insert(toRet.end(), deadPlayers.begin(), deadPlayers.end() );
	return(toRet);
}

Player* PlayerManager::GetPlayerFromID(int id){
	Player* toRet = NULL;
	
	int size = players.size();
	for(int i=0; i<size && toRet == NULL; i++){
		Player* p = players[i];
		if(p->GetId() == id) toRet = p;
	}

	return toRet;
}

Player* PlayerManager::GetPlayerFromNetID(int id){
	Player* toRet = NULL;
	
	int size = players.size();
	for(int i=0; i<size && toRet == NULL; i++){
		Player* p = players[i];
		NetworkObject* nObj = NULL;

		if(p != NULL) nObj = p->GetNetworkObject();
		if(nObj != NULL && nObj->GetObjId() == id) toRet = p;
	}

	return toRet;
}