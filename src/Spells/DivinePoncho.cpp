#include "DivinePoncho.h"
#include "./../Managers/EffectManager.h"
#include "./../Players/Player.h"
#include <SoundEngine/SoundSystem.h>

DivinePoncho::DivinePoncho(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_UNTARGET,
	"./../assets/textures/HUD/Spells/SPELL_UNTARGET.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}


void DivinePoncho::Lanzar(Player* p){
	EffectManager::GetInstance()->AddEffect(p, POWERUP_UNTARGET);
	playSoundEvent(voiceEvent, p->GetPos());
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void DivinePoncho::createSoundEvent() {
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Divina Poncho");
}