#include "Game.h"


Game* Game::instance = 0;

Game::Game(bool isServer){
	masterSpell 	= ControlHechizo::GetInstance();
	masterBullet 	= ControlProyectil::GetInstance();
	masterEffect 	= ControlEffect::GetInstance();
	masterObject	= ControlObject::GetInstance();
	masterPlayer	= ControlPlayer::GetInstance();
	masterTrap		= ControlTrap::GetInstance();

	f_engine = BulletEngine::GetInstance();
	g_engine = GraphicEngine::getInstance();
	s_engine = SoundSystem::getInstance();

	this->isServer = isServer;

	//Start the sound engine
	s_engine->createSystem("./../assets/banks/");
	footstepEvent = s_engine->getEvent("event:/Character/Footsteps/Footsteps"); //Load specific fmod event

	timeStart = GraphicEngine::getInstance()->getTime() * 0.001;

	//Cosas Random--------------------------------------------------------
	/*masterObject->AddSwitch(masterObject->AddDoor());
	masterObject->AddPotion(vector3df(-2, 0, -2));
	masterObject->AddFountain();
	masterObject->AddGrail();
	masterTrap->AddTrap(vector3df(0,-0.5,5),vector3df(1,0,1),TENUM_DEATH_CLAWS);
	masterTrap->AddTrap(vector3df(5,-0.5,0),vector3df(1,0,1),TENUM_SPIRITS);*/

	// START JUGADOR
	if (!isServer){
		playerOne = masterPlayer->AddPlayer(true);
		masterSpell->AddHechizo(0, playerOne, SPELL_PROYECTIL);
	}
	//masterSpell->AddHechizo(1, playerOne, SPELL_BASIC);
	//masterPlayer->AddPlayer(false);

	//masterEffect->AddEffect(playerOne, EFFECT_BURNED);
	// Activacion del timer de ControlHechizo  (Para el deltaTime)
}

Game::~Game(){
	delete masterSpell;
	delete masterBullet;
	delete masterEffect;
	delete masterObject;
	delete masterPlayer;
}

Game* Game::GetInstance(bool isServer){
	if(instance==0){
		instance = new Game(isServer);
	}
	return instance;
}

bool Game::Input(){
	bool end = false;
	
	if(g_engine->IsKeyPressed(KEY_ESCAPE)) {
		end = true;
	}
	if (!isServer){
		if(g_engine->IsLeftButtonPressed()){ 
			ControlHechizo::GetInstance()->ResetHechizo(0,playerOne);
			playerOne->DropObject();
		}
		if(g_engine->IsLeftButtonDown()){  
			ControlHechizo::GetInstance()->LanzarHechizo(0,playerOne); 
		}
		if(g_engine->IsKeyPressed(KEY_KEY_E)){ 
			ControlHechizo::GetInstance()->LanzarHechizo(1,playerOne);
			playerOne->Raycast(); 
		}
		if(g_engine->IsKeyDown(KEY_KEY_E)){ 
			playerOne->Raycast(); 
		}
		if(g_engine->IsKeyPressed(KEY_KEY_Z)){
			playerOne->UseObject();
		}

		if(g_engine->IsKeyDown(KEY_SPACE)){ 
			playerOne->Jump(); 
		}
		if(g_engine->IsKeyDown(KEY_KEY_W)){ 
			playerOne->MoveZ(1); 
		} else if(g_engine->IsKeyDown(KEY_KEY_S)){
			playerOne->MoveZ(-1);
		}
		
		if(g_engine->IsKeyDown(KEY_KEY_A)){ 
			playerOne->MoveX(-1);
			
		}
		else if(g_engine->IsKeyDown(KEY_KEY_D)){ 
			playerOne->MoveX(1); 
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
	}

	return end;
}

void Game::Update(){
	g_engine->UpdateReceiver();
	f_engine->UpdateWorld();
	s_engine->update();

	masterBullet->Update();
	masterSpell->UpdateCooldown();
	masterEffect->UpdateEffects();
	masterObject->Update();
	masterPlayer->UpdatePlayers();
}

void Game::Draw(){
	g_engine->beginSceneDefault(); // Color de borrado en ARGB

	g_engine->drawAll();
	g_engine->drawAim();
	//f_engine->DebugDrawWorld();
}

float Game::GetDeltaTime(){
	return deltaTime;
}

void Game::UpdateDelta(){
	float currentTime = GraphicEngine::getInstance()->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}