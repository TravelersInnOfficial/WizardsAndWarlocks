#ifndef SWITCH_H
#define SWITCH_H

#include <iostream>

#include "./Door.h"
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>

class Switch: public Entidad{
public:
	Switch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	~Switch();
	void SetDoor(Door* d);
	void Update();
	void Contact(void* punt, EntityEnum tipo);

	// Sense Functions
	void SendSignal();
	Kinematic GetKinematic();
private:
	void CreateSwitch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	void UpdatePosShape();

	Door*			actualDoor;
	bool			open;

	BT_Body*        bt_body;
    GBody*          m_switchNode;
};

#endif