#include "GuivernoWind.h"
#include "./../Managers/ObjectManager.h"

GuivernoWind::GuivernoWind(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_BLIZZARD, optHP, optMP){
	area = NULL;
}

void GuivernoWind::Lanzar(Player* p){
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

void GuivernoWind::ResetSpell(){
	Hechizo::ResetSpell();
	if(area!=NULL){
		area->Deactivate();
		area = NULL;
	}
}

void GuivernoWind::CreateArea(Player* p){
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	float dist = 1.5f;
	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	area = ObjectManager::GetInstance()->AddDamageArea(pos, vector3df(1,1,1), vector3df(0,0,0));
}

void GuivernoWind::UpdateArea(Player* p){
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