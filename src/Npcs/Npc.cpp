#include "./Npc.h"

Npc::Npc(){
	pBody = nullptr;
	gBody = nullptr;
	active = false;
}

Npc::~Npc(){
	delete gBody;
	delete pBody;
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