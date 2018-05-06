#include "GuivernoWind.h"
#include "./../Managers/ObjectManager.h"

GuivernoWind::GuivernoWind(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_BLIZZARD,"./../assets/textures/HUD/Spells/SPELL_BLIZZARD.png", optHP, optMP){
	area = nullptr;
	dist = 1.0f;
	timeWaste = 0.0f;
	maxTimeWaste = 0.1f;
	createSoundEvent();
}

GuivernoWind::~GuivernoWind(){
	if(area!=nullptr){
		area->Deactivate();
		area = nullptr;
	}
}

void GuivernoWind::Lanzar(Player* p){
	if(area==nullptr) CreateArea(p);
	else UpdateArea(p);
	
	Hechizo::Lanzar(p);
	if(!lanzable) ResetSpell();
}

void GuivernoWind::WasteMana(Player* p, float deltaTime){
	timeWaste += deltaTime;
	if(timeWaste>=maxTimeWaste){
		timeWaste = 0;
		p->ChangeMP(costePM);
	}
}

void GuivernoWind::DieReset(){
	ResetSpell();
	timeWaste = 0.0f;
}

void GuivernoWind::ResetSpell(){
	shotEvent->setParamValue("Maintain click", 0.90f);
	Hechizo::ResetSpell();
	
	if(area!=nullptr){
		area->Deactivate();
		area = nullptr;
	}
	timeWaste = 0.0f;
}

void GuivernoWind::CreateArea(Player* p){
	vector3df pos = p->GetPos();
	vector3df rot = p->GetCameraRot();

	playSoundEvent(voiceEvent, p->GetPos()); //Play voice event
	shotEvent->setParamValue("Maintain click", 0.00f);
	playSoundEvent(shotEvent, p->GetPos()); //Play voice event

	rot.Z = 0.0f;

	float radRotY = rot.Y * M_PI / 180;
	float radRotX = -rot.X * M_PI / 180;
	pos.X = pos.X + sin(radRotY)*cos(radRotX)*dist;
	pos.Y = pos.Y + sin(radRotX)*dist;
	pos.Z = pos.Z + cos(radRotY)*cos(radRotX)*dist;

	area = (IceArea*)ObjectManager::GetInstance()->AddDamageArea(pos, vector3df(1,0.5f,2), rot, AREA_ICE);
	area->SetEmisor(p->GetId());
}

void GuivernoWind::UpdateArea(Player* p){
	vector3df pos = p->GetPos();
	vector3df rot = p->GetCameraRot();

	float radRotY = rot.Y * M_PI / 180;
	float radRotX = -rot.X * M_PI / 180;
	pos.X = pos.X + sin(radRotY)*cos(radRotX)*dist;
	pos.Y = pos.Y + sin(radRotX)*dist;
	pos.Z = pos.Z + cos(radRotY)*cos(radRotX)*dist;

	rot.Z = 0.0f; 

	area->SetPosition(pos);
	area->SetRotation(rot);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void GuivernoWind::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Guivernum Ventus");
	voiceEvent = SoundSystem::getInstance()->createEvent("event:/Character/Spells/Guivernum Ventus");
}