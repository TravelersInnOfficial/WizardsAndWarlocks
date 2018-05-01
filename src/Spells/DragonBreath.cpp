#include "DragonBreath.h"
#include "./../Managers/BulletManager.h"
#include <BulletCodes.h>

DragonBreath::DragonBreath(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_FIRE,
	"./../assets/textures/HUD/Spells/SPELL_FIRE.png",
	optHP,
	optMP)
{
	createSoundEvent();	
}

void DragonBreath::Lanzar(Player* p){
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	float max = 0;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;

	BulletManager* bulletMananager = BulletManager::GetInstance();
	vector3df direction = vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	bulletMananager->AddProyectil(pos, rot, direction, p->GetId(), p->GetDamageM(), BULLET_FIRE);
	
	playSoundEvent(voiceEvent, pos); //Play voice event
	playSoundEvent(shotEvent, pos); //Play shot event
	
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void DragonBreath::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Dragon Breath");
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Dragon Breath");
}