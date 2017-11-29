#ifndef POTION_H
#define POTION_H

#include <iostream>

#include "./../Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Player;

class Potion:public Entidad{
public:
	Potion(vector3df pos);
	~Potion();
	void Update();
	void Interact(Player* p);
	void Use(Player* p);
	void CreatePotion(vector3df pos);
private:
	void DeletePotion();
	void UpdatePosShape();
	

	int value;

	bool cogida;

	BT_Body*        bt_body;
    GBody*          m_potionNode;
};

#endif