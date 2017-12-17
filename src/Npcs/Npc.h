#ifndef NPC_H
#define NPC_H

#include <iostream>
#include "./../Players/Player.h"

class Npc: public Entidad{
	public:
		Npc();
		void CreatePhysical();
		virtual void Interact(Player* p);
		virtual void Draw();
		virtual ~Npc();

	protected:
		bool activated;
	
};

#endif