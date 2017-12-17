#include "./NpcSelector.h"

NpcSelector::NpcSelector(vector3df TPosition, vector3df TScale, vector3df TRotation){
	CreatePhysical(TPosition, TScale, TRotation);
}

NpcSelector::~NpcSelector(){
}

void NpcSelector::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
	std::cout<<"CREO NPC SELECTOR"<<std::endl;
}

void NpcSelector::Interact(Player* p){
}

void NpcSelector::Draw(){
}