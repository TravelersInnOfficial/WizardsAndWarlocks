#include "SpellDummy.h"

#include "./../Managers/ObjectManager.h"
#include "./../Invocations/Dummy.h"
#include "./../Players/Player.h"
#include <SoundEngine/SoundSystem.h>
#include <Constants.h>
#include <cmath>

SpellDummy::SpellDummy(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_DUMMY,"./../assets/textures/HUD/Spells/SPELL_DUMMY.png", optHP, optMP){
	createSoundEvent();
}
	
SpellDummy::~SpellDummy(){
	if (spawnEvent != nullptr) {
		if (spawnEvent->isPlaying()) spawnEvent->stop();
		spawnEvent->release();
		delete spawnEvent;
	}
}

void SpellDummy::Lanzar(Player* p){	// Estaria bien que se pusiera justo en el suelo
	ObjectManager* objectMaster = ObjectManager::GetInstance();
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	float dist = 1.75f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	pos.Y += 0.25;

	Dummy* target = nullptr;

	if(p->GetAlliance() == ALLIANCE_WIZARD){
		target = (Dummy*)objectMaster->AddInvocation(pos, vector3df(0.27, 0.9, 0.27), rot, INVO_WIZARD);
	}else{
		target = (Dummy*)objectMaster->AddInvocation(pos, vector3df(0.27, 0.9, 0.27), rot, INVO_WARLOCK);
	}

	target->SetName(p->GetName());

	playSoundEvent(voiceEvent, pos); // Play voice event
	playSoundEvent(spawnEvent, pos); // Play spawn event

	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void SpellDummy::createSoundEvent() {
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Dummy");
	spawnEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Dummy");
}