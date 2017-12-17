#include "./Npc.h"

Npc::Npc(){
}

Npc::Npc(vector3df TPosition, vector3df TScale, vector3df TRotation){
	CreatePhysical(TPosition, TScale, TRotation);
}

Npc::~Npc(){
}

void Npc::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
	std::cout<<"CREO NPC GENERICO"<<std::endl;
}

void Npc::Interact(Player* p){
}

void Npc::Draw(){
}