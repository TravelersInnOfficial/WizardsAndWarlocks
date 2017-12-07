#ifndef BT_BODY_H
#define BT_BODY_H

#include "BulletEngine.h"
#include <vector3d.h>
#include <math.h>
#include <iostream>

class BT_Body{
public:

	BT_Body();
	void CreateBox(vector3df position, vector3df dimensions, float Tmass, float roz, vector3df center = vector3df(0.0f,0.0f,0.0f));

	void Update();

	void ApplyCentralImpulse(vector3df);
	void ApplyCentralForce(vector3df);
	void Rotate(vector3df );
	void RotatePos(vector3df, vector3df);

	void SetCenter(vector3df);
	void SetPosition(vector3df);
	void SetDimensions(vector3df);
	void SetLinearVelocity(vector3df);
	void SetMass(float);
	//
	void SetGravity(vector3df);

	vector3df GetPosition();
	vector3df* GetDimensions();
	vector3df* GetLinearVelocity();
	float GetMass();

	//
	void AssignPointer(void*);
	void SetCollisionFlags(std::string);

	void Erase();
	~BT_Body();

private:
	vector3df* m_position;
	vector3df* m_dimensions;
	vector3df* m_center;
	float m_Mass;
	float m_Friction;

	btCollisionShape* m_Shape;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;

	btTransform GetBodyTrans();
};

#endif