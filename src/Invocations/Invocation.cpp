#include "Invocation.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

Invocation::Invocation(int HP){
	m_HP = HP;
	clase = EENUM_INVOCATION;
	createSoundEvent();
}

Invocation::~Invocation(){
	spawnEvent->release();
	delete m_invocationNode;
	delete bt_body;
}

void Invocation::Update(){
	//P KE NO WARNEE
}

bool Invocation::Update(float deltaTime){
	return true;
}

void Invocation::Contact(void* punt, EntityEnum tipo){}

void Invocation::ChangeHP(int value){
	m_HP += value;
	if(m_HP<=0){
		m_HP = 0;
	}
}

void Invocation::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, this, false, AI_INVOCATION, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Invocation::GetKinematic(){
	Kinematic cKin;
	cKin.position = bt_body->GetPosition();
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

void Invocation::createSoundEvent(){
	spawnEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DesperationWallSpawn");
}

void Invocation::playSpawnEvent(vector3df pos){
	SoundSystem::getInstance()->playEvent(spawnEvent, pos);
}