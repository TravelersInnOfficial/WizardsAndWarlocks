#include "PlayerManager.h"
#include "ObjectManager.h"

PlayerManager* PlayerManager::instance = 0;

PlayerManager* PlayerManager::GetInstance(){
	if(instance==0) instance = new PlayerManager();
	return instance;
}

PlayerManager::PlayerManager(){}

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

bool PlayerManager::CheckIfReady(){
	bool allReady = true;
	int size = players.size();
	if(size > 0){
		vector4df readyZone = ObjectManager::GetInstance()->GetReadyZone();
		for(int i=0; i < size && allReady == true; i++){
			Player* p = players[i];
			vector3df pos = p->GetPos();
			if(pos.X < readyZone.X || pos.X > readyZone.X2 || pos.Z < readyZone.Y || pos.Z > readyZone.Y2){
				allReady = false;
			}
		}
	} else allReady = false;

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
}

void PlayerManager::AddToDead(Alliance alliance, Player* player){
	deadPlayers.push_back(player);
	
	if(alliance == ALLIANCE_WIZARD){
		deadWizards.push_back(player);
		if(deadWizards.size() == wizardPlayers.size()) std::cout<<"WARLOCKS WIN"<<std::endl;
	}

	else{
		deadWarlocks.push_back(player);
		if(deadWarlocks.size() == warlockPlayers.size()) std::cout<<"WIZARDS WIN"<<std::endl;
	}

	/*std::cout<<"#######################################"<<std::endl;
	std::cout<<"Players: "<<players.size()<<std::endl;
	std::cout<<"Dead Players: "<<deadPlayers.size()<<std::endl;
	std::cout<<"-----------------"<<std::endl;
	std::cout<<"Wizards: "<<wizardPlayers.size()<<std::endl;
	std::cout<<"Dead Wizards: "<<deadWizards.size()<<std::endl;
	std::cout<<"-----------------"<<std::endl;
	std::cout<<"Warlocks: "<<warlockPlayers.size()<<std::endl;
	std::cout<<"Dead Warlocks: "<<deadWarlocks.size()<<std::endl;*/
}

void PlayerManager::ChangeAlliance(Alliance alliance, Player* player){
	wizardPlayers.erase(std::remove(wizardPlayers.begin(), wizardPlayers.end(), player), wizardPlayers.end());
	warlockPlayers.erase(std::remove(warlockPlayers.begin(), warlockPlayers.end(), player), warlockPlayers.end());
	
	if(alliance == ALLIANCE_WIZARD) wizardPlayers.push_back(player);
	else warlockPlayers.push_back(player);

	/*std::cout<<"#######################################"<<std::endl;
	std::cout<<"Players: "<<players.size()<<std::endl;
	std::cout<<"Dead Players: "<<deadPlayers.size()<<std::endl;
	std::cout<<"-----------------"<<std::endl;
	std::cout<<"Wizards: "<<wizardPlayers.size()<<std::endl;
	std::cout<<"Dead Wizards: "<<deadWizards.size()<<std::endl;
	std::cout<<"-----------------"<<std::endl;
	std::cout<<"Warlocks: "<<warlockPlayers.size()<<std::endl;
	std::cout<<"Dead Warlocks: "<<deadWarlocks.size()<<std::endl;*/
}