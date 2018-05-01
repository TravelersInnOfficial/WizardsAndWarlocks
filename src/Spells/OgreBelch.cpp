#include "OgreBelch.h"
#include "./../Managers/BulletManager.h"

OgreBelch::OgreBelch(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM, 
	tCast, 
	tCoolDown, 
	SPELL_POISON, 
	"./../assets/textures/HUD/Spells/SPELL_POISON.png",
	optHP, 
	optMP)
{
	createSoundEvent();
}

void OgreBelch::Lanzar(Player* p){
	vector3df pos = p->GetHeadPos();
	vector3df rot = p->GetRot();
	rot.X = -rot.X;

	float max = 1;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;
	vector3df direction = vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));

	BulletManager* bullman = BulletManager::GetInstance();
	bullman->AddProyectil(pos, rot, direction, p->GetId(), p->GetDamageM(), POISON_BOMB);
	
	playSoundEvent(voiceEvent, pos); //Play voice event
	playSoundEvent(shotEvent, pos); //Play shot event
	
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void OgreBelch::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Eructam Troglodytam");
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Eructam Troglodytam");
}