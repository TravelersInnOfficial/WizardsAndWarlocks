#ifndef NPCSELLER_H
#define NPCSELLER_H

#include "./Npc.h"

class NpcSeller: public Npc{
	friend class ShopMenu;
	
	public:
		NpcSeller(vector3df TPosition, vector3df TScale, vector3df TRotation);
		~NpcSeller();

		void CreateVisual(vector3df TPosition, vector3df TScale, vector3df TRotation);
		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		void Interact(Player* p);
		void ShowInteractInfo(Player* p);

};

#endif