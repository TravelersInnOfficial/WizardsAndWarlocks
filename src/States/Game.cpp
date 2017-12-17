#include "Game.h"

Game::Game(){

	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();

	f_engine 		= BulletEngine::GetInstance();
	g_engine 		= GraphicEngine::getInstance();
	s_engine 		= SoundSystem::getInstance();

	// Level
	LevelLoader loader;
	loader.LoadLevel("../assets/json/Lobby.json");
	objectManager->AddNpc(vector3df(1,-1,1), vector3df(1,1,1), vector3df(0,0,0), NPC_SELECTOR);

	// Sound Engine
	s_engine->createSystem("./../assets/banks/");
	footstepEvent = s_engine->getEvent("event:/Character/Footsteps/Footsteps");

	// Graphic Engine
	timeStart = GraphicEngine::getInstance()->getTime() * 0.001;
	g_engine->addCameraSceneNodeFPS(120.f, 0.0f);

	// Jugador
	playerOne = (HumanPlayer*) playerManager->AddHumanPlayer();
	spellManager->AddHechizo(0, playerOne, SPELL_PROYECTIL);
	spellManager->AddHechizo(1, playerOne, SPELL_BASIC);
	
	//playerManager->AddAIPlayer();
	//effectManager->AddEffect(playerOne, EFFECT_BURNED);
}

Game::~Game(){
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
}

bool Game::Input(){
	bool end = false;
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) end = true;
	if(g_engine->IsKeyPressed(KEY_KEY_F)) playerOne->DeployTrap();
	if(g_engine->IsKeyPressed(KEY_KEY_P)) playerOne->ChangeHP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+3);
	if(g_engine->IsKeyPressed(KEY_KEY_R)) playerOne->Respawn();

	if(g_engine->IsKeyPressed(KEY_KEY_H)){
		LevelLoader loader;
		ObjectManager::GetInstance()->ClearMap();
		loader.LoadLevel("../assets/json/map.json");
	}

	if(g_engine->IsKeyPressed(KEY_KEY_A) || g_engine->IsKeyPressed(KEY_KEY_W) || g_engine->IsKeyPressed(KEY_KEY_S) || g_engine->IsKeyPressed(KEY_KEY_D)){
		if(!footstepEvent->isPlaying()) footstepEvent->start();
		s_engine->setListenerPosRot(playerOne->GetPos(), playerOne->GetRot());
		
	}
	else if (g_engine->IsKeyUp(KEY_KEY_A) && g_engine->IsKeyUp(KEY_KEY_W) && g_engine->IsKeyUp(KEY_KEY_S) && g_engine->IsKeyUp(KEY_KEY_D)){
		if(footstepEvent->isPlaying()) footstepEvent->stop();
	}

	return end;
}

void Game::Update(){
	UpdateDelta();

	f_engine->UpdateWorld();
	s_engine->update();

	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	playerManager->UpdatePlayers();
	trapManager->Update(deltaTime);

	g_engine->UpdateReceiver();

	setFps();
}

void Game::setFps(){
	secondCounter += deltaTime;
	if(secondCounter >= 0.5){
		secondCounter = 0;
		std::string myFps = to_string(int(1/deltaTime));
		std::wstring wsTmp(myFps.begin(), myFps.end());
		g_engine->ChangeWindowName(wsTmp);
	}
}

void Game::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim();
	if(playerOne != NULL) g_engine->drawManaAndHealth(playerOne->GetHP(), playerOne->GetMP());
	f_engine->DebugDrawWorld();
}

float Game::GetTotalTime(){ return GraphicEngine::getInstance()->getTime(); }

float Game::GetDeltaTime(){ return deltaTime; }

void Game::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}