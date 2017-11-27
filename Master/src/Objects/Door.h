#ifndef DOOR_H
#define DOOR_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Door: public Entidad{
public:
	Door();
	~Door();
	void Interact(Player* p);
	void Interact();
	void Update();
private:
	void CreateDoor();
	void WorkDoor();
	void UpdatePosShape();

	bool working;
	int min;
	int max;
	int increment;

	vector3df rotation;

	BT_Body*        bt_body;
    GBody*          m_doorNode;
};

#endif