#include "BulletEngine.h"
#include "BT_GhostObject.h"
#include <iostream>
#include "../Trap.h"

static BulletEngine* instance;

BulletEngine* BulletEngine::GetInstance() {
    if (instance == NULL) instance = new BulletEngine();
    return (instance);
}

BulletEngine::BulletEngine(){

	m_broadphase = NULL;
	m_collisionConfiguration = NULL;
	m_dispatcher = NULL;
	m_solver = NULL;
	m_dynamicsWorld = NULL;
	m_groundShape = NULL;
	m_groundMotionState = NULL;
    m_groundRigidBody = NULL;

}

void BulletEngine::CreateWorld(){
	//NEEDS FOR CREATING THE WORLD
	m_broadphase = new btDbvtBroadphase();

	m_collisionConfiguration = new btDefaultCollisionConfiguration();       
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_solver = new btSequentialImpulseConstraintSolver;
	
	//INSTANTIATES THE DYNAMICS WORLD
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	
	//SETS THE GRAVITY
	m_dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	
	//CREATES THE GROUND
	m_groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	//GROUND INITIAL MOTION STATE
	m_groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	//CREATE GROUND INFO
    btRigidBody::btRigidBodyConstructionInfo
        groundRigidBodyCI(0, m_groundMotionState, m_groundShape, btVector3(0, 0, 0));

    //AGREGATE THE GROUND INFO TO THE GROUND RIGID BODY
    m_groundRigidBody = new btRigidBody(groundRigidBodyCI);

    //ADD THE GROUND TO THE WORLD
    //m_dynamicsWorld->addRigidBody(m_groundRigidBody);

	m_dynamicsWorld->setInternalTickCallback(motorPreTickCallback,this ,true);
}

void BulletEngine::UpdateWorld(){

	m_dynamicsWorld->stepSimulation(1 / 60.f, 7);

}

void BulletEngine::EraseWorld(){
    m_dynamicsWorld->removeRigidBody(m_groundRigidBody);
    delete m_groundRigidBody->getMotionState();
    delete m_groundRigidBody;

    delete m_groundShape;

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_broadphase;
}

 void BulletEngine::AddRigidBody(btRigidBody* rigidBody){
 	m_dynamicsWorld->addRigidBody(rigidBody);
 }

 void BulletEngine::motorPreTickCallback (btDynamicsWorld *world, btScalar timeStep){
	std::cout<<"The world just ticked by "<<(float)timeStep<<" seconds\n"<<std::endl;

	BulletEngine *w = static_cast<BulletEngine *>(world->getWorldUserInfo());
	w->motorProcessCallback(timeStep);
}

void BulletEngine::motorProcessCallback(btScalar timeStep){
	btCollisionObjectArray WorldObjects = m_dynamicsWorld->getCollisionObjectArray();
	for (int i = 0; i < WorldObjects.size(); i++) {
		//btRigidBody *rigidBody = btRigidBody::upcast(WorldObjects[i]);
		btGhostObject *ghostObject = btGhostObject::upcast(WorldObjects[i]);
		//BT_GhostObject *ghostObject = BT_GhostObject::upcast(WorldObjects[i]);
               if (!ghostObject) {
                   continue;
               }
		//TYPE 2 = RIGIDBODY; TYPE 4 = GHOSTBODY
		std::cout<<"Object: "<< i <<", Type: "<< WorldObjects[i]->getInternalType()<< " == "<< WorldObjects[i]->CO_GHOST_OBJECT <<" (Ghost?)"<<std::endl;

		for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++){
			btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(ghostObject->getOverlappingObject(i));
			// do whatever you want to do with these pairs of colliding objects
			std::cout<<"Something stepped on me!!!"<<std::endl;

			//std::cout<<ghostObject->getUserPointer()<<std::endl;
			Entidad* ghost = (Entidad*)(ghostObject->getUserPointer());
			ghost->Contact(ghost, ghost->GetClase());

			//std::cout<<ghostObject->getOverlappingObject(i)->getInternalType()<<std::endl;
		}	
	}
}

 void BulletEngine::AddGhostBody(btGhostObject* ghostBody){
	m_dynamicsWorld->addCollisionObject(ghostBody);
	m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
 }

 void BulletEngine::RemoveRigidBody(btRigidBody* rigidBody){
 	m_dynamicsWorld->removeRigidBody(rigidBody);
 }

 void BulletEngine::RemoveGhostObject(btGhostObject* ghostBody){
	 m_dynamicsWorld->removeCollisionObject(ghostBody);

 }

BulletEngine::~BulletEngine(){}