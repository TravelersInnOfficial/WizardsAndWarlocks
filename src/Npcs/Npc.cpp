#include "./Npc.h"

Npc::Npc(){
}

Npc::~Npc(){
	pBody->Erase();
    gBody->Erase();	

	delete gBody;
	delete pBody;
}

void Npc::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
}

void Npc::Interact(Player* p){
}

void Npc::Draw(){
}