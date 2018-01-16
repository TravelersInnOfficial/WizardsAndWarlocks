#include "Teleport.h"
#include "./../PhysicsEngine/BulletEngine.h"

Teleport::Teleport(float costPM, float tCast, float tCoolDown, float optHP, float optMP)
:Hechizo(
	costPM,
	tCast,
	tCoolDown,
	SPELL_FIRE,
	"./../assets/textures/HUD/Spells/SPELL_TELEPORT.png",
	optHP,
	optMP)
{
	distTeleport = 5.0f;
	createSoundEvent();	
}

void Teleport::Lanzar(Player* p){
	vector3df rot = p->GetRot();

	rot.X = -rot.X;

	vector3df pos = p->GetHeadPos();
	vector3df end(
				pos.X + sin(rot.Y)*cos(rot.X)*distTeleport,
				pos.Y + sin(rot.X)*distTeleport,
				pos.Z + cos(rot.Y)*cos(rot.X)*distTeleport
	);

	vector3df point;
	vector3df normal;

	if(!BulletEngine::GetInstance()->Raycast(pos, end, &point, &normal)){
		point = end;
	}

	p->SetPosition(point);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void Teleport::createSoundEvent() {
	shotEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DragonBreathShot");
}