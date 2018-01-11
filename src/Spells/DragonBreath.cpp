#include "DragonBreath.h"
#include "./../Managers/BulletManager.h"

DragonBreath::DragonBreath(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_FIRE,
	"./../assets/textures/HUD/Spells/fireball.png",
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

	BulletManager* bullman = BulletManager::GetInstance();
	vector3df direction = vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	bullman->AddProyectil(pos, direction, p->GetId(), p->GetDamageM(), BULLET_FIRE);
	playShotEvent(pos); //Play sound event
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void DragonBreath::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}