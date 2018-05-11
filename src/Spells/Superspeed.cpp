#include "Superspeed.h"
#include "./../Managers/EffectManager.h"
#include "./../Players/Player.h"
#include <SoundEngine/SoundSystem.h>

Superspeed::Superspeed(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_SPEED,
	"./../assets/textures/HUD/Spells/SPELL_SPEED.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void Superspeed::Lanzar(Player* p){
	EffectManager::GetInstance()->AddEffect(p, POWERUP_SPEED);
	playSoundEvent(voiceEvent, p->GetPos()); //Play voice event
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void Superspeed::createSoundEvent() {
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Hermes' Slippers");
}