#ifndef BT_BODY_H
#define BT_BODY_H

#include "BulletEngine.h"
#include "../vector3d.h"
#include <math.h>

class BT_Body{
public:

	BT_Body();
	void CreateBox(vector3df position, vector3df dimensions, float, float);

	void Update();

	void ApplyCentralImpulse(vector3df);
	void Rotate(vector3df rotation);

	void SetPosition(vector3df);
	void SetDimensions(vector3df);
	void SetLinearVelocity(vector3df);
	void SetMass(float);
	void SetUserPtr(void*);

	vector3df* GetPosition();
	vector3df* GetDimensions();
	vector3df* GetLinearVelocity();
	float GetMass();

	void Erase();
	~BT_Body();

private:
	vector3df* m_position;
	vector3df* m_dimensions;
	float m_Mass;
	float m_Friction;

	btCollisionShape* m_Shape;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;

	btTransform GetBodyTrans();
};

#endif