#include "TeleportBase.h"
#include "./../Managers/ObjectManager.h"

TeleportBase::TeleportBase(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(costPM, tCast, tCoolDown, SPELL_WALL,"./../assets/textures/HUD/Spells/SPELL_TELEPORTBASE.png", optHP, optMP){
	base = NULL;
	justPlaced = false;
}

void TeleportBase::DieReset(){
	if(base!=NULL){
		base->Deactivate();
		base = NULL;
		Hechizo::DieReset();
	}
}

void TeleportBase::Lanzar(Player* p){	// Estaria bien que se pusiera justo en el suelo
	if(base == NULL){
		vector3df rot = p->GetRot();
		rot.X = 0; rot.Y = 0;
		vector3df pos = p->GetPos();

		base = (BaseT*)ObjectManager::GetInstance()->AddInvocation(pos, vector3df(1,1,1), rot, INVO_TELEPORT);

		justPlaced = true;
	}
	else{
		vector3df pos = base->GetPosition();
		p->SetPosition(pos);

		base->Deactivate();
		base = NULL;
		justPlaced = true;
	}
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void TeleportBase::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}