#include "Superspeed.h"
#include "./../Managers/EffectManager.h"

Superspeed::Superspeed(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_FIRE,
	"./../assets/textures/HUD/Spells/SPELL_SPEED.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void Superspeed::Lanzar(Player* p){
	EffectManager::GetInstance()->AddEffect(p, POWERUP_SPEED);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void Superspeed::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}