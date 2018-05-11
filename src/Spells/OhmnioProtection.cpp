#include "OhmnioProtection.h"
#include "./../Managers/EffectManager.h"
#include "./../Players/Player.h"
#include <SoundEngine/SoundSystem.h>

OhmnioProtection::OhmnioProtection(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_DEFENSE,
	"./../assets/textures/HUD/Spells/SPELL_DEFENSE.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void OhmnioProtection::Lanzar(Player* p){
	EffectManager::GetInstance()->AddEffect(p, POWERUP_DEFENSE);
	playSoundEvent(voiceEvent, p->GetPos()); //Play voice event
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void OhmnioProtection::createSoundEvent() {
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Ohmio Protection");
}