#ifndef NPCSELLER_H
#define NPCSELLER_H

#include <iostream>
#include <ColliderMasks.h>
#include <Menus.h>
#include <Alliance.h>
#include <KeyStates.h>
#include <TrapCodes.h>
#include <SpellCodes.h>
#include "./Npc.h"
#include "./../Players/Player.h"
#include "./../Players/HumanPlayer.h"
#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/MenuManager.h>

class NpcSeller: public Npc{
	friend class ShopMenu;
	
	public:
		NpcSeller(vector3df TPosition, vector3df TScale, vector3df TRotation);
		~NpcSeller();

		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		void Interact(Player* p);
		void ShowInteractInfo();

	private:
		HumanPlayer* hp;
};

#endif