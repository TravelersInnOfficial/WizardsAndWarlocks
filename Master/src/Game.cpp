#include "Game.h"

Game* Game::instance = 0;

Game::Game(){
	masterSpell 	= ControlHechizo::GetInstance();
	masterBullet 	= ControlProyectil::GetInstance();
	masterEffect 	= ControlEffect::GetInstance();
	masterObject	= ControlObject::GetInstance();
	masterPlayer	= ControlPlayer::GetInstance();

	//Cosas Random--------------------------------------------------------
	masterObject->AddSwitch(masterObject->AddDoor());
	masterObject->AddPotion(vector3df(-2, 0, -2));
	masterObject->AddFountain();
	masterObject->AddGrial();

	// START JUGADOR
	playerOne = masterPlayer->AddPlayer(true);
	masterSpell->AddHechizo(0, playerOne, SPELL_PROYECTIL);
	masterSpell->AddHechizo(1, playerOne, SPELL_BASIC);
	masterPlayer->AddPlayer(false);

	masterEffect->AddEffect(playerOne, EFFECT_BURNED);
	// Activacion del timer de ControlHechizo  (Para el deltaTime)
	masterSpell->StartTime();
	masterEffect->StartTime();
}

Game::~Game(){
	delete masterSpell;
	delete masterBullet;
	delete masterEffect;
	delete masterObject;
	delete masterPlayer;
}

Game* Game::GetInstance(){
	if(instance==0){
		instance = new Game();
	}
	return instance;
}

bool Game::Input(){
	GraphicEngine* engine = GraphicEngine::getInstance();
	bool end = false;
	
	if(engine->IsKeyPressed(KEY_ESCAPE)) {
		end = true;
	}
	if(engine->IsLeftButtonPressed()){ 
		ControlHechizo::GetInstance()->ResetHechizo(0,playerOne);
		playerOne->DropObject();
	}
	if(engine->IsLeftButtonDown()){  
		ControlHechizo::GetInstance()->LanzarHechizo(0,playerOne); 
	}
	if(engine->IsKeyPressed(KEY_KEY_E)){ 
		ControlHechizo::GetInstance()->LanzarHechizo(1,playerOne);
		playerOne->Raycast(); 
	}
	if(engine->IsKeyPressed(KEY_KEY_Z)){
		playerOne->UseObject();
	}

	if(engine->IsKeyDown(KEY_SPACE)){ 
		playerOne->Jump(); 
	}
	if(engine->IsKeyDown(KEY_KEY_W)){ 
		playerOne->MoveZ(1); 
	} else if(engine->IsKeyDown(KEY_KEY_S)){
		playerOne->MoveZ(-1);
	}
	
	if(engine->IsKeyDown(KEY_KEY_A)){ 
		playerOne->MoveX(-1); 
	}
	else if(engine->IsKeyDown(KEY_KEY_D)){ 
		playerOne->MoveX(1); 
	}

	if(engine->IsKeyPressed(KEY_KEY_P)){ 
		playerOne->ChangeHP(-5); 
	}
	else if(engine->IsKeyPressed(KEY_KEY_O)){ 
		playerOne->ChangeHP(+3); 
	}

	if(engine->IsKeyDown(KEY_KEY_R)){ 
		playerOne->Respawn(); 
	}
	return end;
}

void Game::Update(){
	masterBullet->Update();
	masterSpell->UpdateCooldown();
	masterEffect->UpdateEffects();
	masterObject->Update();
	masterPlayer->UpdatePlayers();
}

void Game::Draw(){}