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
	/*
	if(!isPlayerOne){
		m_position = networkObject->GetVecFVar(PLAYER_POSITION);
		bt_body->SetPosition(m_position);
	}
	else m_position = bt_body->GetPosition();
	*/

	CheckInput();
	Player::Update();

	// if(isPlayerOne) networkObject->SetVecFVar(PLAYER_POSITION, m_position, true, false);
}