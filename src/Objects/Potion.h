#ifndef POTION_H
#define POTION_H

#include <iostream>
#include <ColliderMasks.h>

#include "./../Players/Player.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../PhysicsEngine/BT_Body.h"

class Player;

class Potion:public Entidad{
public:
	Potion(vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Potion();
	void Update();
	void Interact(Player* p);
	void Use(Player* p);
	void CreatePotion(vector3df TPosition, vector3df TRotation);
	void Drop(vector3df force);

private:
	int 		value;
	bool 		picked;
	vector3df 	scale;			// Variable almacenada para poder volver a formar el cuerpo una vez eliminado

	BT_Body*    bt_body;
    GBody*      m_potionNode;

private:
	void DeletePotion();
	void UpdatePosShape();
};

#endif