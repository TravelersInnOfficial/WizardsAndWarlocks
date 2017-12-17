#ifndef NPCSELECTOR_H
#define NPCSELECTOR_H

#include <iostream>
#include "./Npc.h"
#include "./../Players/Player.h"

class NpcSelector: public Npc{
	public:
		NpcSelector();
		~NpcSelector();

		void CreatePhysical();
		void Interact(Player* p);
		void Draw();
};

#endif