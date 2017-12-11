#include "AIPlayer.h"
#include "./Managers/PlayerManager.h"

AIPlayer::AIPlayer():Player(false){
	controller = new BehaviourTree();
	controller->AnyadirInformacion(AI_CHARACTER, this);
	controller->AnyadirInformacion(AI_TARGET, PlayerManager::GetInstance()->GetOne());
}

AIPlayer::~AIPlayer(){
	delete controller;
}

void AIPlayer::Update(){
	Player::Update();
	controller->run();
}

void AIPlayer::SetForces(vector3df v){
	bt_body->ApplyCentralImpulse(v);
}

void AIPlayer::SetAngularForce(vector3df v){
	bt_body->ApplyTorque(v);
}