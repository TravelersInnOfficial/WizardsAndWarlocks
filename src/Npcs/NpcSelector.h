#ifndef NPCSELECTOR_H
#define NPCSELECTOR_H

#include <iostream>
#include <ColliderMasks.h>
#include "./Npc.h"
#include "./../Players/Player.h"
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class NpcSelector: public Npc{
	public:
		NpcSelector(vector3df TPosition, vector3df TScale, vector3df TRotation);
		~NpcSelector();

		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		void Interact(Player* p);
		void Draw();
};

#endif