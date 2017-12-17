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
		virtual void Draw();

	protected:
	    GBody*      gBody;
    	BT_Body*    pBody;
		bool activated;
	
};

#endif