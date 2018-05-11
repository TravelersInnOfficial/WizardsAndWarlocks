#ifndef SWITCH_H
#define SWITCH_H

#include "./../Entidad.h"

class Door;
class GBody;
class BT_Body;
class Kinematic;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

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