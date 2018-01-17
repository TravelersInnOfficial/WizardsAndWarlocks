#include "InvisibilityCape.h"
#include "../Managers/EffectManager.h"

InvisibilityCape::InvisibilityCape(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_INVISIBILITY,
	"./../assets/textures/HUD/Spells/SPELL_INVISIBILITY.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}

void InvisibilityCape::Lanzar(Player* p){
    EffectManager::GetInstance()->AddEffect(p, POWERUP_INVISIBLE);
    Hechizo::Lanzar(p);
}

void InvisibilityCape::createSoundEvent(){

}