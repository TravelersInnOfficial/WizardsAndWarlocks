#include "HumanPlayer.h"

#include "./Managers/SpellManager.h"

HumanPlayer::HumanPlayer(bool isPlayer1):Player(isPlayer1){
	controller = new PlayerController();
	DeclareInput();
}

HumanPlayer::~HumanPlayer(){
	delete controller;
}

void HumanPlayer::DeclareInput(){
	controller->AddAction(KEY_KEY_W, ACTION_MOVE_UP);
	controller->AddAction(KEY_KEY_S, ACTION_MOVE_DOWN);
	controller->AddAction(KEY_KEY_A, ACTION_MOVE_LEFT);
	controller->AddAction(KEY_KEY_D, ACTION_MOVE_RIGHT);
	controller->AddAction(KEY_KEY_E, ACTION_RAYCAST);
	controller->AddAction(KEY_SPACE, ACTION_JUMP);
	controller->AddAction(KEY_KEY_Z, ACTION_USE_OBJECT);
	controller->AddAction(KEY_LBUTTON, ACTION_SHOOT);
}

void HumanPlayer::UpdateInput(){
	controller->Update();
}

void HumanPlayer::SetNetInput(){
	if(controller->IsKeyPressed(ACTION_MOVE_LEFT)) networkObject->SetIntVar(MOVE_LEFT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_LEFT)) networkObject->SetIntVar(MOVE_LEFT, 4, true, false);

	if(controller->IsKeyPressed(ACTION_MOVE_DOWN)) networkObject->SetIntVar(MOVE_DOWN, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_DOWN)) networkObject->SetIntVar(MOVE_DOWN, 4, true, false);

	if(controller->IsKeyPressed(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(MOVE_RIGHT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(MOVE_RIGHT, 4, true, false);

	if(controller->IsKeyPressed(ACTION_MOVE_UP)) networkObject->SetIntVar(MOVE_UP, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_UP)) networkObject->SetIntVar(MOVE_UP, 4, true, false);

	if(controller->IsKeyPressed(ACTION_RAYCAST)) networkObject->SetIntVar(RAYCAST, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_RAYCAST)) networkObject->SetIntVar(RAYCAST, 4, true, false);

	if(controller->IsKeyPressed(ACTION_JUMP)) networkObject->SetIntVar(JUMP, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_JUMP)) networkObject->SetIntVar(JUMP, 4, true, false);

	if(controller->IsKeyPressed(ACTION_USE_OBJECT)) networkObject->SetIntVar(USE_OBJECT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_USE_OBJECT)) networkObject->SetIntVar(USE_OBJECT, 4, true, false);

	if(controller->IsKeyDown(ACTION_SHOOT)) networkObject->SetIntVar(SHOOT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_SHOOT)) networkObject->SetIntVar(SHOOT, 4, true, false);
}

void HumanPlayer::GetNetInput(){
	int keystate = 0;
	keystate = networkObject->GetIntVar(MOVE_LEFT);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_LEFT, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(MOVE_RIGHT);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_RIGHT, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(MOVE_UP);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_UP, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(MOVE_DOWN);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_DOWN, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(RAYCAST);
	if(keystate != 0) controller->SetStatus(ACTION_RAYCAST, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(JUMP);
	if(keystate != 0) controller->SetStatus(ACTION_JUMP, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(USE_OBJECT);
	if(keystate != 0) controller->SetStatus(ACTION_USE_OBJECT, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(SHOOT);
	if(keystate != 0) controller->SetStatus(ACTION_SHOOT, (keyStatesENUM)keystate);
}

void HumanPlayer::CheckInput(){
	if(controller->IsKeyPressed(ACTION_MOVE_LEFT)){ this->MoveX(-1); }
	if(controller->IsKeyPressed(ACTION_MOVE_DOWN)){ this->MoveZ(-1); }
	if(controller->IsKeyPressed(ACTION_MOVE_RIGHT)){ this->MoveX(1); }
	if(controller->IsKeyPressed(ACTION_MOVE_UP)){ this->MoveZ(1); }
	if(controller->IsKeyPressed(ACTION_RAYCAST)){ this->Raycast(); }
	if(controller->IsKeyDown(ACTION_JUMP)){ this->Jump(); }
	if(controller->IsKeyDown(ACTION_USE_OBJECT)){ this->UseObject(); }
	if(controller->IsKeyDown(ACTION_SHOOT)){ 
		SpellManager::GetInstance()->ResetHechizo(0,this); 
		this->DropObject();
	}
	if(controller->IsKeyPressed(ACTION_SHOOT)){ SpellManager::GetInstance()->LanzarHechizo(0,this); }
}

void HumanPlayer::Update(){
	CheckInput();
	Player::Update();
}