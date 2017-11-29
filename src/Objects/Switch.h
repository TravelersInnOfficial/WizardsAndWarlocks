#ifndef SWITCH_H
#define SWITCH_H

#include <iostream>

#include "./Door.h"
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"

class Switch: public Entidad{
public:
	Switch();
	~Switch();
	void SetDoor(Door* d);
	void Update();
	void Contact(void* punt, EntityEnum tipo);
private:
	void CreateSwitch();
	void UpdatePosShape();

	Door*			actualDoor;
	bool			open;

	BT_Body*        bt_body;
    GBody*          m_switchNode;
};

#endif