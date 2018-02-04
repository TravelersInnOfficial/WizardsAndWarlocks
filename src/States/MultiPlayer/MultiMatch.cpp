#include "MultiMatch.h"

MultiMatch::MultiMatch(MultiPlayerGame* fat){
	father = fat;

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();
	n_engine 		= NetworkEngine::GetInstance();

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	// Level
	LevelLoader::LoadLevel("../assets/json/Map1.json");

	// Ponemos a true el inicio de la partida de los players
	playerManager->ManageMatchStatus(true);

	// Quitamos los menus en el caso de que se haya quedado abierto
	g_engine->ToggleMenu(false);
	MenuManager::GetInstance()->ClearMenu();

	if(n_engine->IsServerInit()) isServer = true;
	else if(n_engine->IsClientInit()) isServer = false;

	// Asignamos el playerOne
	playerOne = playerManager->GetPlayerOne();
	networkObject = networkManager->GetMultiGame();

	if(networkObject!=NULL && isServer)networkObject->SetBoolVar(MULTIGAME_CHANGE, false, true, false);
}

MultiMatch::~MultiMatch(){
	// Los motores los eliminara el StateManager
	// Los managers los eliminara el SinglePlayerManager
}

void MultiMatch::CheckIfWon(){
	Alliance whosWon = NO_ALLIANCE;

	if(isServer){
		if(objectManager->CheckIfWon() || playerManager->CheckIfWon(ALLIANCE_WIZARD)){
			whosWon = ALLIANCE_WIZARD;
		}
		else if (playerManager->CheckIfWon(ALLIANCE_WARLOCK)) whosWon = ALLIANCE_WARLOCK;
		
		if(whosWon != NO_ALLIANCE) {
			MatchEnded(whosWon);
		} 
	}
}

void MultiMatch::MatchEnded(Alliance winnerAlliance){
	
	if(isServer){ 
		//n_engine->GetServer()->EndMatch(winnerAlliance);
		networkObject->SetBoolVar(MULTIGAME_BACK_LOBBY, true, true, false);
	}

	//Play sound event when you lose or win
	/*if (playerOne != NULL) {
		if (playerOne->GetAlliance() != winnerAlliance)  playEvent(soundEvents["defeat"]);
		else playEvent(soundEvents["victory"]);
	}*/
	//GraphicEngine::getInstance()->InitReceiver();
	//playerManager->EraseAllCharacters();
	//if(playerOne != NULL) {
		//MenuManager::GetInstance()->CreateMenu(ENDMATCH_M, (int)winnerAlliance);
		//g_engine->ToggleMenu(true);
	//	playerOne->SetAllInput(UP);
	//}
	
}

bool MultiMatch::Input(){
	return false;
}

void MultiMatch::Update(float deltaTime){

	n_engine->Update();
	f_engine->UpdateWorld();

	if(g_engine->getActiveCamera() != NULL){
		s_engine->Update(g_engine->getActiveCamera()->getPosition(), g_engine->getActiveCamera()->getRotation());
	}

	playerManager->UpdatePlayers(deltaTime, true);
	networkManager->Update();
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	trapManager->Update(deltaTime);
	MenuManager::GetInstance()->UpdateNetDebug();

	g_engine->UpdateReceiver();

	if(networkObject!=NULL && networkObject->GetBoolVar(MULTIGAME_BACK_LOBBY)){
		father->ReturnLobby();
	}

	CheckIfWon();
}

void MultiMatch::Draw(){
	if(playerOne != NULL){
		g_engine->drawAim(playerOne->GetMoving());
		playerOne->Draw();
	}
	objectManager->DrawGrailGUI();
	g_engine->drawAllGUI();	// Draws the MENU (if one is activated)
}