#ifndef NPCSELECTOR_H
#define NPCSELECTOR_H

#include "./Npc.h"

class NpcSelector: public Npc{
	public:
		NpcSelector(vector3df TPosition, vector3df TScale, vector3df TRotation);
		~NpcSelector();

		void CreateVisual(vector3df TPosition, vector3df TScale, vector3df TRotation);
		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		void Interact(Player* p);
		void ShowInteractInfo();
};

#endif