#include "NetGame.h"

NetGame* NetGame::instance = NULL;

NetGame* NetGame::GetInstance(){
	if(instance == NULL) instance = new NetGame();
	return instance;
}

NetGame::NetGame(){
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	networkManager	= NetworkManager::GetInstance();

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();
	n_engine 		= NetworkEngine::GetInstance();

	if(n_engine->IsServerInit()) isServer = true;
	else if(n_engine->IsClientInit()) isServer = false;

	// Sound Engine
	s_engine->createSystem("./../assets/banks/");
	footstepEvent = s_engine->getEvent("event:/Character/Footsteps/Footsteps");

	// Graphic Engine
	timeStart = GraphicEngine::getInstance()->getTime() * 0.001;
	g_engine->addCameraSceneNodeFPS(120.f, 0.005);

	// Jugador
	playerOne = NULL;
}

NetGame::~NetGame(){
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
	delete networkManager;
}

bool NetGame::Input(){
	bool end = false;
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) end = true;
	
	if (g_engine->IsKeyUp(KEY_KEY_A) && g_engine->IsKeyUp(KEY_KEY_W) && g_engine->IsKeyUp(KEY_KEY_S) && g_engine->IsKeyUp(KEY_KEY_D)){
		if(footstepEvent->isPlaying()) footstepEvent->stop();
	}

	if(playerOne != NULL){
		playerOne->UpdateInput();
		if(g_engine->IsKeyPressed(KEY_KEY_F)) playerOne->DeployTrap();
		if(g_engine->IsKeyPressed(KEY_KEY_P)) playerOne->ChangeHP(-5);
		if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+3);
		if(g_engine->IsKeyDown(KEY_KEY_R)) playerOne->Respawn();
		if(g_engine->IsKeyPressed(KEY_KEY_A) || g_engine->IsKeyPressed(KEY_KEY_W) || g_engine->IsKeyPressed(KEY_KEY_S) || g_engine->IsKeyPressed(KEY_KEY_D)){
			if(!footstepEvent->isPlaying()) footstepEvent->start();
		}
	}

	return end;
}

void NetGame::Update(){
	UpdateDelta();

	n_engine->Update();
	g_engine->UpdateReceiver();
	f_engine->UpdateWorld();
	s_engine->update();

	networkManager->Update();
	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	trapManager->Update(deltaTime);

	playerManager->UpdatePlayers();
}

void NetGame::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim();
}

float NetGame::GetTotalTime(){ return GraphicEngine::getInstance()->getTime(); }

float NetGame::GetDeltaTime(){ return deltaTime; }

void NetGame::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}

void NetGame::SetPlayerOne(NetworkObject* nObject){
	std::cout<<"CREAMOS PLAYER"<<std::endl;
	if(!isServer && playerOne == NULL) {
		std::cout<<"ES PLAYER ONE"<<std::endl;
		playerOne = playerManager->AddPlayer(true);
		playerOne->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, playerOne, SPELL_PROYECTIL);
		GraphicEngine::getInstance()->addCameraSceneNodeFPS(120.f, 0.f);
	}
	else{
		std::cout<<"ES PLAYER TWO"<<std::endl;
		Player* newPlayer = playerManager->AddPlayer(false);
		newPlayer->SetNetworkObject(nObject);
		spellManager->AddHechizo(0, newPlayer, SPELL_PROYECTIL);
	}
}