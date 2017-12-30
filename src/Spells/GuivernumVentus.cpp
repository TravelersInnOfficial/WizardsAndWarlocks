#include "GuivernumVentus.h"
#include "./../Managers/ObjectManager.h"

GuivernumVentus::GuivernumVentus(int costPM, float tCast, float tCoolDown)
:Hechizo(costPM, tCast, tCoolDown){
	area = NULL;
}

void GuivernumVentus::Lanzar(Player* p){
	if(area==NULL){
		CreateArea(p);
	}
	else{
		UpdateArea(p);
	}
	Hechizo::Lanzar(p);

	if(!lanzable){
		ResetSpell();
	}
}

void GuivernumVentus::ResetSpell(){
	Hechizo::ResetSpell();
	if(area!=NULL){
		area->Deactivate();
		area = NULL;
	}
}

void GuivernumVentus::CreateArea(Player* p){
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	float dist = 1.5f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	area = ObjectManager::GetInstance()->AddDamageArea(pos, vector3df(1,1,1), vector3df(0,0,0));
}

void GuivernumVentus::UpdateArea(Player* p){
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	float dist = 1.5f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	area->SetPosition(pos);
	area->SetRotation(rot);
}

float GuivernumVentus::GetUtility(Player* p){
	float HP = p->GetHP();
	float MP = p->GetMP();

	HP = HP / 100;		// Saco los porcentajes [0, 1]
	MP = MP / 100; 
	// Situacion optima 50HP && 100MP
	float utility = HP + MP;
	utility = utility / 2;
	return utility;
}