#include "Game.h"


Game* Game::instance = 0;

Game::Game(){
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();
	trapManager		= TrapManager::GetInstance();

	f_engine = BulletEngine::GetInstance();
	g_engine = GraphicEngine::getInstance();
	s_engine = SoundSystem::getInstance();

	//Start the sound engine
	s_engine->createSystem("./../assets/banks/");
	footstepEvent = s_engine->getEvent("event:/Character/Footsteps/Footsteps"); //Load specific fmod event

	timeStart = GraphicEngine::getInstance()->getTime() * 0.001;

	//Cosas Random--------------------------------------------------------
	objectManager->AddSwitch(
		objectManager->AddDoor(vector3df(1,0,-1), vector3df(0.05, 1, 0.5), vector3df(0,0,0), vector3df(0,0,-0.5)),
		vector3df(-1, 0, -4), vector3df(1,1,1), vector3df(0,0,0), vector3df(0,0,0)
		);
	objectManager->AddPotion(vector3df(-2, 0, -2), vector3df(0.5, 0.5, 0.5), vector3df(0,0,0));
	objectManager->AddFountain();
	objectManager->AddGrail();

	//Traps
	trapManager->AddTrap(vector3df(0,-0.49,5),TENUM_DEATH_CLAWS);
	trapManager->AddTrap(vector3df(5,-0.49,0),TENUM_SPIRITS);

	// START JUGADOR
	playerOne = playerManager->AddPlayer(true);
	spellManager->AddHechizo(0, playerOne, SPELL_PROYECTIL);
	spellManager->AddHechizo(1, playerOne, SPELL_BASIC);
	playerManager->AddPlayer(false);

	//effectManager->AddEffect(playerOne, EFFECT_BURNED);
	// Activacion del timer de ControlHechizo  (Para el deltaTime)
}

Game::~Game(){
	delete spellManager;
	delete bulletManager;
	delete effectManager;
	delete objectManager;
	delete playerManager;
}

Game* Game::GetInstance(){
	if(instance==0){
		instance = new Game();
	}
	return instance;
}

bool Game::Input(){
	bool end = false;
	
	playerOne->UpdateInput();
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) {
		end = true;
	}
	
	
	if(g_engine->IsKeyPressed(KEY_KEY_F)){
		playerOne->DeployTrap();
	}


	if(g_engine->IsKeyPressed(KEY_KEY_P)){ 
		playerOne->ChangeHP(-5); 
	}
	else if(g_engine->IsKeyPressed(KEY_KEY_O)){ 
		playerOne->ChangeHP(+3); 
	}

	if(g_engine->IsKeyDown(KEY_KEY_R)){ 
		playerOne->Respawn(); 
	}

	if(g_engine->IsKeyPressed(KEY_KEY_A) || g_engine->IsKeyPressed(KEY_KEY_W) || g_engine->IsKeyPressed(KEY_KEY_S) || g_engine->IsKeyPressed(KEY_KEY_D) ) {
		if(!footstepEvent->isPlaying()){ //Start the footsteps sound
			footstepEvent->start();
		}
				
			
	}

	if (g_engine->IsKeyUp(KEY_KEY_A) && g_engine->IsKeyUp(KEY_KEY_W) && g_engine->IsKeyUp(KEY_KEY_S) && g_engine->IsKeyUp(KEY_KEY_D) ) {
		if(footstepEvent->isPlaying()) { //Stop the footstep sound
			footstepEvent->stop();
		}
				
	}

	return end;
}

void Game::Update(){
	g_engine->UpdateReceiver();
	f_engine->UpdateWorld();
	s_engine->update();

	bulletManager->Update();
	spellManager->UpdateCooldown();
	effectManager->UpdateEffects();
	objectManager->Update();
	playerManager->UpdatePlayers();
}

void Game::Draw(){
	g_engine->beginSceneDefault(); // Color de borrado en ARGB

	g_engine->drawAll();
	g_engine->drawAim();
	f_engine->DebugDrawWorld();
}

float Game::GetTotalTime(){
	return GraphicEngine::getInstance()->getTime();
}

float Game::GetDeltaTime(){
	return deltaTime;
}

void Game::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}