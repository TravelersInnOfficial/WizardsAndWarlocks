#ifndef POTION_H
#define POTION_H

#include <iostream>

#include "./../Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Potion{
public:
	Potion();
	void Update();
	void Interact(Player* p);
	void Use(Player* p);
private:
	void UpdatePosShape();
	void CreatePotion();

	int value;

	BT_Body*        bt_body;
    GBody*          m_potionNode;
};

#endif