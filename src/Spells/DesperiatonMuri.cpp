#include "DesperiatonMuri.h"
#include "./../Managers/ObjectManager.h"

DesperiatonMuri::DesperiatonMuri(int costPM, float tCast, float tCoolDown):Hechizo(costPM, tCast, tCoolDown){}
	
void DesperiatonMuri::Lanzar(Player* p){	// Estaria bien que se pusiera justo en el suelo
	ObjectManager* objectMaster = ObjectManager::GetInstance();
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	float dist = 1.5f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	objectMaster->AddInvocation(pos, vector3df(1,1,1), rot);

	Hechizo::Lanzar(p);
}

float DesperiatonMuri::GetUtility(Player* p){
	float HP = p->GetHP();
	float MP = p->GetMP();

	HP = HP / 100;		// Saco los porcentajes [0, 1]
	MP = MP / 100; 
	// Situacion optima 50HP && 100MP
	float utility = HP + MP;
	utility = utility / 2;
	return utility;
}
