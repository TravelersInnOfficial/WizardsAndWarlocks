#include "OgreBelch.h"
#include "./../Managers/BulletManager.h"

OgreBelch::OgreBelch(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM, 
	tCast, 
	tCoolDown, 
	SPELL_POISON, 
	"",
	optHP, 
	optMP)
{
	
}

void OgreBelch::Lanzar(Player* p){
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	float max = 0;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;

	BulletManager* bullman = BulletManager::GetInstance();
	vector3df direction = vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X));
	bullman->AddProyectil(pos, direction, p->GetId(), p->GetDamageM(), POISON_BOMB);

	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void OgreBelch::createSoundEvent() {
	
}