#include "OhmnioProtection.h"
#include "./../Managers/EffectManager.h"

OhmnioProtection::OhmnioProtection(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_FIRE,
	"./../assets/textures/HUD/Spells/SPELL_DEFENSE.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void OhmnioProtection::Lanzar(Player* p){
	EffectManager::GetInstance()->AddEffect(p, POWERUP_DEFENSE);
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void OhmnioProtection::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}