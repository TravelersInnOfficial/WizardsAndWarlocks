#include "SpellDummy.h"
#include "./../Managers/ObjectManager.h"

SpellDummy::SpellDummy(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_DUMMY,"./../assets/textures/HUD/Spells/SPELL_DUMMY.png", optHP, optMP){
	
}
	
void SpellDummy::Lanzar(Player* p){	// Estaria bien que se pusiera justo en el suelo
	ObjectManager* objectMaster = ObjectManager::GetInstance();
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	float dist = 1.75f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	pos.Y += 0.25;

	if(p->GetAlliance() == ALLIANCE_WIZARD){
		objectMaster->AddInvocation(pos, vector3df(0.54, 1.62, 0.54), rot, INVO_WIZARD);
	}else{
		objectMaster->AddInvocation(pos, vector3df(0.54, 1.62, 0.54), rot, INVO_WARLOCK);
	}

	Hechizo::Lanzar(p);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/