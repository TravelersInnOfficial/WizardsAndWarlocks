#ifndef NPC_H
#define NPC_H

#include <iostream>
#include "./../Players/Player.h"

class Npc: public Entidad{
	public:
		Npc();
		Npc(vector3df TPosition, vector3df TScale, vector3df TRotation);
		virtual ~Npc();
		
		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		virtual void Interact(Player* p);
		virtual void Draw();

	protected:
		bool activated;
	
};

#endif