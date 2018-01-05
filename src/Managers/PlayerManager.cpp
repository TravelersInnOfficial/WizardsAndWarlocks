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
	wizardPlayers.clear();
	warlockPlayers.clear();	
	deadPlayers.clear();
	deadWizards.clear();
	deadWarlocks.clear();	
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
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		if(isNetGame) {
			HumanPlayer* hp = (HumanPlayer*) players[i];
			if(p->IsPlayerOne()) hp->SetNetInput();
			else hp->GetNetInput();
		}
		p->Update();
	}
}

void PlayerManager::AddToDeletePlayer(int networkId){
	int size = players.size();
	for(int i=0; i<size; i++){
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

// Comprobamos TODOS las variables READY (Solo para RED)
bool PlayerManager::CheckIfReady(){
	bool allReady = true;
	int size = players.size();
	
	// Si alguno de los personajes no esta READY
	// Devolvemos una FALSE
	for(int i=0; i < size && allReady == true; i++){
		Player* p = players[i];
		if(!p->GetReadyStatus()) allReady = false;
	}

	if(size < 2) allReady = false;
	return allReady;
}

void PlayerManager::RespawnAll(){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->Respawn();
	}
}

void PlayerManager::ManageMatchStatus(bool started){
	int size = players.size();
	for(int i=0; i<size; i++){
		Player* p = players[i];
		p->SetMatchStatus(started);
	}

	deadPlayers.clear();
	deadWizards.clear();
	deadWarlocks.clear();
}

void PlayerManager::AddToDead(Alliance alliance, Player* player){
	deadPlayers.push_back(player);
	
	if(alliance == ALLIANCE_WIZARD){
		deadWizards.push_back(player);
		if(!wizardsWin && !warlocksWin && deadWizards.size() == wizardPlayers.size()) warlocksWin = true;
	}

	else{
		deadWarlocks.push_back(player);
		if(!wizardsWin && !warlocksWin && deadWarlocks.size() == warlockPlayers.size()) wizardsWin = true;
	}
}

void PlayerManager::ChangeAlliance(Alliance alliance, Player* player){
	wizardPlayers.erase(std::remove(wizardPlayers.begin(), wizardPlayers.end(), player), wizardPlayers.end());
	warlockPlayers.erase(std::remove(warlockPlayers.begin(), warlockPlayers.end(), player), warlockPlayers.end());
	
	if(alliance == ALLIANCE_WIZARD) wizardPlayers.push_back(player);
	else warlockPlayers.push_back(player);
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

void PlayerManager::UpdateNetDebug(){
	MenuManager::GetInstance()->UpdateNetDebug(players);
}