#ifndef BT_BODY_H
#define BT_BODY_H

#include "BulletEngine.h"

class BT_Body{
public:

	BT_Body();
	void CreateBox(float, float, float, float, float, float, float);

	void Update();

	void ApplyCentralImpulse(float, float, float);

	void SetPosition(float, float, float);
	void SetPosX(float);
	void SetPosY(float);
	void SetPosZ(float);

	float GetPosX();
	float GetPosY();
	float GetPosZ();

	void Erase();
	~BT_Body();

private:

	float m_posX;
	float m_posY;
	float m_posZ;
	float m_width;
	float m_height;
	float m_length;

	btCollisionShape* m_Shape;
	btDefaultMotionState* m_MotionState;
	btScalar m_Mass;
	btRigidBody* m_RigidBody;

	btTransform GetBodyTrans();

};

#endif