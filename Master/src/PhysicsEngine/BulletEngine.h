#ifndef BULLETENGINE_H
#define BULLETENGINE_H

#include <bullet/btBulletDynamicsCommon.h>

class BulletEngine{
public:

	static BulletEngine* GetInstance();
	
	void CreateWorld();
    void UpdateWorld();
    void EraseWorld();
    void AddRigidBody(btRigidBody*);
    void RemoveRigidBody(btRigidBody*);

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


};

#endif
