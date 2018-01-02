#include "OgreBelch.h"
#include "./../Managers/BulletManager.h"

OgreBelch::OgreBelch(int costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_POISON, optHP, optMP){
	
}

void OgreBelch::Lanzar(Player* p){
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	float max = 0;
	pos.X = pos.X + sin(rot.Y)*cos(rot.X)*max;
	pos.Y = pos.Y + sin(rot.X)*max;
	pos.Z = pos.Z + cos(rot.Y)*cos(rot.X)*max;

	BulletManager* c = BulletManager::GetInstance();
	c->AddProyectil(pos, vector3df( sin(rot.Y)*cos(rot.X), sin(rot.X), cos(rot.Y)*cos(rot.X)), 0.05f, 30.0f, p->GetId(), BULLET_BASIC);

	Hechizo::Lanzar(p);
}