#include "DivinePoncho.h"
#include "./../Managers/EffectManager.h"

DivinePoncho::DivinePoncho(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_FIRE,
	"./../assets/textures/HUD/Spells/SPELL_UNTARGET.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void DivinePoncho::Lanzar(Player* p){
	EffectManager::GetInstance()->AddEffect(p, POWERUP_UNTARGET);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void DivinePoncho::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}