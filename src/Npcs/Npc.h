#ifndef NPC_H
#define NPC_H

#include <iostream>
#include "./../Players/Player.h"

class Npc: public Entidad{
	public:
		Npc();
		virtual ~Npc();
		
		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		virtual void Interact(Player* p);
		virtual void StopInteraction();
		virtual void Update();
		void Draw();

	protected:
		BT_Body*	pBody;
	    GBody*      gBody;
		bool active;
	
};

#endif