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
	if(area==nullptr){
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
	vector3df rot = p->GetRot();

	playSoundEvent(voiceEvent, p->GetPos()); //Play voice event
	shotEvent->setParamValue("Maintain click", 0.00f);
	playSoundEvent(shotEvent, p->GetPos()); //Play voice event

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	area = (IceArea*)ObjectManager::GetInstance()->AddDamageArea(pos, vector3df(1,1,1), vector3df(0,0,0), AREA_ICE);
	area->SetEmisor(p->GetId());
}

void GuivernoWind::UpdateArea(Player* p){
	vector3df pos = p->GetPos();
	vector3df rot = p->GetRot();

	pos.X = pos.X + sin(rot.Y)*dist;
	pos.Z = pos.Z + cos(rot.Y)*dist;

	rot.Z = 0.0f; rot.X = 0.0f;
	rot.Y = rot.Y * 180 / M_PI;

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