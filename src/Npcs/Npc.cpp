#include "./Npc.h"

#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GAnimation.h>

Npc::Npc(){
	pBody = nullptr;
	gBody = nullptr;
	active = false;
}

Npc::~Npc(){
	if(gBody != nullptr) delete gBody;
	if(pBody != nullptr) delete pBody;
}

void Npc::Update(float deltaTime){
	if(gBody != nullptr) gBody->Update(deltaTime);
}

void Npc::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
}

void Npc::Interact(Player* p){
}

void Npc::StopInteraction(){
}