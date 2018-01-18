#include "GaiaCleanse.h"
#include "./../Managers/EffectManager.h"

GaiaCleanse::GaiaCleanse(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_CLEANSE,
	"./../assets/textures/HUD/Spells/SPELL_CLEANSE.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void GaiaCleanse::Lanzar(Player* p){
	EffectManager::GetInstance()->CleanEffects(p);
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void GaiaCleanse::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}