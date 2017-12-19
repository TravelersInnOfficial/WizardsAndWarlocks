#ifndef DOOR_H
#define DOOR_H

#include <iostream>

#include "./../Entidad.h"
#include "./../Players/Player.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>

class Door: public Entidad{
public:
	Door(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	~Door();
	void Interact(Player* p);
	void Interact();
	void Update();
private:
	void CreateDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
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