#ifndef BULLETENGINE_H
#define BULLETENGINE_H

#include <bullet/btBulletDynamicsCommon.h>
#include "./BulletDebug.h"
#include "./../Entidad.h"

class BulletEngine{
public:

	static BulletEngine* GetInstance();
	
	void CreateWorld();
    void UpdateWorld();
    void EraseWorld();
    void AddRigidBody(btRigidBody*);
    void RemoveRigidBody(btRigidBody*);
    //
    void CheckColisions();
    void DebugDrawWorld();

	virtual ~BulletEngine();

private:

	BulletEngine();

	btBroadphaseInterface* m_broadphase;

	btDefaultCollisionConfiguration* m_collisionConfiguration;      
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	
	btCollisionShape* m_groundShape;

	btDefaultMotionState* m_groundMotionState;

    btRigidBody* m_groundRigidBody;

    BulletDebug* m_physicsDebug;

};

#endif
