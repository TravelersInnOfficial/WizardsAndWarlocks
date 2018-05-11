#ifndef NPC_H
#define NPC_H

#include <iostream>
#include "./../Entidad.h"

class Player;
class BT_Body;
class GAnimation;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Npc: public Entidad{
	public:
		Npc();
		virtual ~Npc();
		
		void Update(float deltaTime);
		void CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation);
		virtual void Interact(Player* p);
		virtual void StopInteraction();

	protected:
		BT_Body*		pBody;
	    GAnimation*   	gBody;
		bool active;
	
};

#endif