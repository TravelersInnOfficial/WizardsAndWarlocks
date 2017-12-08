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

	// Sound Engine
	s_engine->createSystem("./../assets/banks/");
	footstepEvent = s_engine->getEvent("event:/Character/Footsteps/Footsteps");

	// Graphic Engine
	timeStart = GraphicEngine::getInstance()->getTime() * 0.001;
	g_engine->addCameraSceneNodeFPS(120.f, 0.0f);

	// Otras Cosas
	objectManager->AddSwitch(
		objectManager->AddDoor(vector3df(1,0,-1), vector3df(0.05, 1, 0.5), vector3df(0,0,0), vector3df(0,0,-0.5)),
		vector3df(-1, 0, -4), vector3df(1,1,1), vector3df(0,0,0), vector3df(0,0,0)
		);
	objectManager->AddPotion(vector3df(-2, 0, -2), vector3df(0.5, 0.5, 0.5), vector3df(0,0,0));
	objectManager->AddFountain();
	objectManager->AddGrail();

	// Trampas
	trapManager->AddTrap(vector3df(0,-0.49,5),TENUM_DEATH_CLAWS);
	trapManager->AddTrap(vector3df(5,-0.49,0),TENUM_SPIRITS);

	// Jugador
	playerOne = (HumanPlayer*) playerManager->AddHumanPlayer();
	spellManager->AddHechizo(0, playerOne, SPELL_PROYECTIL);
	spellManager->AddHechizo(1, playerOne, SPELL_BASIC);

	playerManager->AddAIPlayer();
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
	
	playerOne->UpdateInput();
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) end = true;
	if(g_engine->IsKeyPressed(KEY_KEY_F)) playerOne->DeployTrap();
	if(g_engine->IsKeyPressed(KEY_KEY_P)) playerOne->ChangeHP(-5);
	if(g_engine->IsKeyPressed(KEY_KEY_O)) playerOne->ChangeHP(+3);
	if(g_engine->IsKeyDown(KEY_KEY_R)) playerOne->Respawn();

	if(g_engine->IsKeyPressed(KEY_KEY_A) || g_engine->IsKeyPressed(KEY_KEY_W) || g_engine->IsKeyPressed(KEY_KEY_S) || g_engine->IsKeyPressed(KEY_KEY_D)){
		if(!footstepEvent->isPlaying()) footstepEvent->start();
	}
	else if (g_engine->IsKeyUp(KEY_KEY_A) && g_engine->IsKeyUp(KEY_KEY_W) && g_engine->IsKeyUp(KEY_KEY_S) && g_engine->IsKeyUp(KEY_KEY_D)){
		if(footstepEvent->isPlaying()) footstepEvent->stop();
	}

	return end;
}

void Game::Update(){
	UpdateDelta();

	g_engine->UpdateReceiver();
	f_engine->UpdateWorld();
	s_engine->update();

	bulletManager->Update();
	spellManager->UpdateCooldown(deltaTime);
	effectManager->UpdateEffects(deltaTime);
	objectManager->Update(deltaTime);
	playerManager->UpdatePlayers();
	trapManager->Update(deltaTime);
}

void Game::Draw(){
	g_engine->beginSceneDefault();
	g_engine->drawAll();
	g_engine->drawAim();
	f_engine->DebugDrawWorld();
}

float Game::GetTotalTime(){ return GraphicEngine::getInstance()->getTime(); }

float Game::GetDeltaTime(){ return deltaTime; }

void Game::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}