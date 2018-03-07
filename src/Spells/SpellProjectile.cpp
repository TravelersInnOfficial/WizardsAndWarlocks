#include "SpellProjectile.h"
#include "./../Managers/BulletManager.h"

SpellProjectile::SpellProjectile(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM, 
	tCast, 
	tCoolDown, 
	SPELL_PROJECTILE,
	"./../assets/textures/HUD/Spells/SPELL_PROJECTILE_WIZARD.png", 
	optHP, 
	optMP)
{
	createSoundEvent();	
}

void SpellProjectile::Lanzar(Player* p){
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	float max = 0;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;

	BulletManager* bullman = BulletManager::GetInstance();
	vector3df direction = vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	bullman->AddProyectil(pos, direction, p->GetId(), p->GetDamageM(), BULLET_BASIC, p->m_shotEffect);
	playSoundEvent(shotEvent,pos); //Play sound event
	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void SpellProjectile::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Regular Shot");
}