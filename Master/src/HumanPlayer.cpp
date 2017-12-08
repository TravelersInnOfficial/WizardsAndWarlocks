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
	//controller->AddAction(KEY_LBUTTON, ACTION_SHOOT);
	controller->AddAction(KEY_KEY_Q, ACTION_SHOOT);
}

void HumanPlayer::UpdateInput(){
	controller->Update();
}

void HumanPlayer::SetNetInput(){
	if(controller->IsKeyDown(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 4, true, false);

	if(controller->IsKeyDown(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 4, true, false);

	if(controller->IsKeyDown(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 4, true, false);

	if(controller->IsKeyDown(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 4, true, false);

	if(controller->IsKeyDown(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 4, true, false);

	if(controller->IsKeyDown(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 4, true, false);

	if(controller->IsKeyDown(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 4, true, false);

	if(controller->IsKeyDown(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 2, true, false);
	else if(controller->IsKeyPressed(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 3, true, false);
	else if(controller->IsKeyReleased(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 4, true, false);

	networkObject->SetVecFVar(PLAYER_POSITION, GetPos(), true, false);
	networkObject->SetVecFVar(PLAYER_ROTATION, GetRot(), true, false);
}

void HumanPlayer::GetNetInput(){
	int keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_MOVE_LEFT);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_LEFT, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_MOVE_RIGHT);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_RIGHT, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_MOVE_UP);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_UP, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_MOVE_DOWN);
	if(keystate != 0) controller->SetStatus(ACTION_MOVE_DOWN, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_RAYCAST);
	if(keystate != 0) controller->SetStatus(ACTION_RAYCAST, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_JUMP);
	if(keystate != 0) controller->SetStatus(ACTION_JUMP, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_USE_OBJECT);
	if(keystate != 0) controller->SetStatus(ACTION_USE_OBJECT, (keyStatesENUM)keystate);

	keystate = 0;
	keystate = networkObject->GetIntVar(PLAYER_SHOOT);
	if(keystate != 0) controller->SetStatus(ACTION_SHOOT, (keyStatesENUM)keystate);

	vector3df objstate = vector3df(0,0,0);
	objstate = networkObject->GetVecFVar(PLAYER_POSITION);
	SetPosition(objstate);

	objstate = vector3df(0,0,0);
	objstate = networkObject->GetVecFVar(PLAYER_ROTATION);
	SetRotation(objstate);

}

void HumanPlayer::CheckInput(){
	if(controller->IsKeyDown(ACTION_MOVE_LEFT)){ this->MoveX(-1); }
	if(controller->IsKeyDown(ACTION_MOVE_DOWN)){ this->MoveZ(-1); }
	if(controller->IsKeyDown(ACTION_MOVE_RIGHT)){ this->MoveX(1); }
	if(controller->IsKeyDown(ACTION_MOVE_UP)){ this->MoveZ(1); }
	if(controller->IsKeyDown(ACTION_RAYCAST)){ this->Raycast(); }
	if(controller->IsKeyPressed(ACTION_JUMP)){ this->Jump(); }
	if(controller->IsKeyPressed(ACTION_USE_OBJECT)){ this->UseObject(); }
	if(controller->IsKeyPressed(ACTION_SHOOT)){ 
		SpellManager::GetInstance()->ResetHechizo(0,this); 
		this->DropObject();
	}
	if(controller->IsKeyDown(ACTION_SHOOT)){ SpellManager::GetInstance()->LanzarHechizo(0,this); }
}

void HumanPlayer::Update(){
	CheckInput();
	Player::Update();
}