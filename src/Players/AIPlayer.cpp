#include "AIPlayer.h"
#include "./../Managers/PlayerManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../GraphicEngine/GraphicEngine.h"

AIPlayer::AIPlayer():Player(false){
	controller = new BehaviourTree();
	controller->AnyadirInformacion(AI_CHARACTER, this);

	RegionalSenseManager* senseManager = RegionalSenseManager::GetInstance();
	senseManager->AddSensor(&m_position, &rotation, 0.0f, controller->GetBlackboard());
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
	bt_body->SetAngularVelocity(v);
}

void AIPlayer::Debug(){
	
	vector3df p = m_position;
	vector3df l = rotation;
	vector3df c = vector3df(1,1,1);

	vector3df o = vector3df(p.X+sin(l.Y+0.5235)*3, p.Y + sin(-l.X+0.5235)*3, p.Z+cos(l.Y+0.5235)*3 );
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);
	o = vector3df(p.X+sin(l.Y-0.5235)*3, p.Y + sin(-l.X+0.5235)*3, p.Z+cos(l.Y-0.5235)*3 );
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);
	o = vector3df(p.X+sin(l.Y+0.5235)*3, p.Y + sin(-l.X-0.5235)*3, p.Z+cos(l.Y+0.5235)*3 );
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);
	o = vector3df(p.X+sin(l.Y-0.5235)*3, p.Y + sin(-l.X-0.5235)*3, p.Z+cos(l.Y-0.5235)*3 );
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);


	float lookAHead = 2.5f;
	float lookAHead2 = 1.0f;
	c = vector3df(0,1,0);
	o = vector3df(p.X+sin(l.Y)*lookAHead, p.Y, p.Z+cos(l.Y)*lookAHead); 
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);
	o = vector3df(p.X+sin(l.Y + M_PI/4)*lookAHead2, p.Y, p.Z+cos(l.Y + M_PI/4)*lookAHead2);
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);
	o = vector3df(p.X+sin(l.Y - M_PI/4)*lookAHead2, p.Y, p.Z+cos(l.Y - M_PI/4)*lookAHead2);
	GraphicEngine::getInstance()->paintLineDebug(p, o, c);

}