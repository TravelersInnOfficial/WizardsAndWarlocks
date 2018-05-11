#ifndef BULLETENGINE_H
#define BULLETENGINE_H

#include <ColliderMasks.h>

class btSequentialImpulseConstraintSolver;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;
class btCollisionDispatcher;
struct btDefaultMotionState;
class btBroadphaseInterface;
class btCollisionShape;
class btDynamicsWorld;
class btGhostObject;
class BulletDebug;
class btRigidBody;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;
typedef float btScalar;

class BulletEngine{
public:

	static BulletEngine* GetInstance();

	void CreateWorld();
    void UpdateWorld(float deltaTime);
    void EraseWorld();

    void AddRigidBody(btRigidBody*, CollisionTypes mask = C_NOTHING, int collideWith = 0);
	void AddGhostBody(btGhostObject*, CollisionTypes mask = C_NOTHING, int collideWith = 0);

    void RemoveRigidBody(btRigidBody*);
    void RemoveGhostObject(btGhostObject*);

    void DebugDrawWorld();

    void* Raycast(vector3df Start, vector3df End, int collisionFilter = raycastCW);
    bool Raycast(vector3df Start, vector3df End, vector3df* point, vector3df* normal, int collisionFilter = raycastCW);

	static void motorPreTickCallback(btDynamicsWorld*, btScalar);
	void motorProcessCallback(btScalar);

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
