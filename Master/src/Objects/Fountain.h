#ifndef FOUNTAIN_H
#define FOUNTAIN_H


#include <iostream>

#include "./../Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Fountain{
public:
	Fountain();
	void Update();
	void Interact(Player* p);
	void SetFree();
private:
	void Recover();
	bool Use();
	void UpdatePosShape();
	void CreateFountain();

	bool 		inUse;
	Player* 	user;
	int  		maxValue;
	int 		value;
	int 		incrementUse;
	int 		incrementValue;

	BT_Body*        bt_body;
    GBody*          m_fountainNode;
};

#endif