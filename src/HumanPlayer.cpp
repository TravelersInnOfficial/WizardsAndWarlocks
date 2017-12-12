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

	controller->AddAction(KEY_KEY_F, ACTION_DEPLOY_TRAP);
}

void HumanPlayer::UpdateInput(){
	controller->UpdateOwnStatus();
	if(isPlayerOne) controller->Update();
}

void HumanPlayer::SetNetInput(){
	if(controller->IsKeyPressed(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 3, true, false);

	if(controller->IsKeyPressed(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 3, true, false);

	if(controller->IsKeyPressed(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 3, true, false);

	if(controller->IsKeyPressed(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 3, true, false);

	if(controller->IsKeyPressed(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 3, true, false);

	if(controller->IsKeyPressed(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 3, true, false);

	if(controller->IsKeyPressed(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 3, true, false);

	if(controller->IsKeyPressed(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 3, true, false);

	if(controller->IsKeyPressed(ACTION_DEPLOY_TRAP)) networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, 2, true, false);
	else if(controller->IsKeyReleased(ACTION_DEPLOY_TRAP)) networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, 3, true, false);

	networkObject->SetVecFVar(PLAYER_POSITION, GetPos(), true, false);
	networkObject->SetVecFVar(PLAYER_ROTATION, GetRot(), true, false);
}

void HumanPlayer::GetNetInput(){
	int keystate = -1;
	vector3df objstate = vector3df(99999,0,0);

	keystate = networkObject->GetIntVar(PLAYER_MOVE_LEFT);
	if(keystate != -1){
		controller->SetStatus(ACTION_MOVE_LEFT, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_MOVE_LEFT, keystate, false, false);
	}
	
	keystate = networkObject->GetIntVar(PLAYER_MOVE_RIGHT);
	if(keystate != -1){
		controller->SetStatus(ACTION_MOVE_RIGHT, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_MOVE_RIGHT, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_MOVE_UP);
	if(keystate != -1){
		controller->SetStatus(ACTION_MOVE_UP, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_MOVE_UP, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_MOVE_DOWN);
	if(keystate != -1){
		controller->SetStatus(ACTION_MOVE_DOWN, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_MOVE_DOWN, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_RAYCAST);
	if(keystate != -1){
		controller->SetStatus(ACTION_RAYCAST, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_RAYCAST, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_JUMP);
	if(keystate != -1){
		controller->SetStatus(ACTION_JUMP, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_JUMP, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_USE_OBJECT);
	if(keystate != -1){
		controller->SetStatus(ACTION_USE_OBJECT, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_USE_OBJECT, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_SHOOT);
	if(keystate != -1){
		controller->SetStatus(ACTION_SHOOT, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_SHOOT, keystate, false, false);
	}

	keystate = networkObject->GetIntVar(PLAYER_DEPLOY_TRAP);
	if(keystate != -1){
		controller->SetStatus(ACTION_DEPLOY_TRAP, (keyStatesENUM)keystate);
		keystate = -1;
		networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, keystate, false, false);
	}
	
	objstate = networkObject->GetVecFVar(PLAYER_POSITION);
	if(objstate.X != 99999){
		SetPosition(objstate);
		objstate = vector3df(99999,0,0);
		networkObject->SetVecFVar(PLAYER_POSITION, objstate, false, false);
	}

	objstate = networkObject->GetVecFVar(PLAYER_ROTATION);
	if(objstate.X != 99999){
		SetRotation(objstate);
		objstate = vector3df(99999,0,0);
		networkObject->SetVecFVar(PLAYER_ROTATION, objstate, false, false);
	}

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
	if(controller->IsKeyPressed(ACTION_DEPLOY_TRAP)){this->DeployTrap();}
}

void HumanPlayer::Update(){
	CheckInput();
	Player::Update();
	UpdateInput();
}