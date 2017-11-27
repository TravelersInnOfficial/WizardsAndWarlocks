#include "BulletEngine.h"
#include "./../GraphicEngine/GraphicEngine.h"

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
    m_physicsDebug = NULL;
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

    //ADD THE PHYSICS DEBUG
    m_physicsDebug = new BulletDebug();
    m_dynamicsWorld->setDebugDrawer(m_physicsDebug);

	//ADDS THE MOTOR TICK CALLBACK
	m_dynamicsWorld->setInternalTickCallback(motorPreTickCallback,this ,true);

}

void BulletEngine::UpdateWorld(){

	m_dynamicsWorld->stepSimulation(1 / 60.f, 7);
	CheckColisions();
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
    delete m_physicsDebug;
}

void BulletEngine::AddRigidBody(btRigidBody* rigidBody){
	m_dynamicsWorld->addRigidBody(rigidBody);
}

void BulletEngine::AddGhostBody(btGhostObject* ghostBody){
	m_dynamicsWorld->addCollisionObject(ghostBody);
	m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
}

void BulletEngine::RemoveRigidBody(btRigidBody* rigidBody){
	//m_dynamicsWorld->removeCollisionObject(rigidBody);
	m_dynamicsWorld->removeRigidBody(rigidBody);
}

void BulletEngine::RemoveGhostObject(btGhostObject* ghostBody){
	 m_dynamicsWorld->removeCollisionObject(ghostBody);

}

void BulletEngine::CheckColisions(){
	int numManifolds = m_dispatcher->getNumManifolds();
	for(int i=0; i<numManifolds; i++){
		btPersistentManifold* contactManifold = m_dispatcher->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();
		
		void* objetoA = obA->getUserPointer();
		void* objetoB = obB->getUserPointer();

		int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++) {
            btManifoldPoint& pt = contactManifold->getContactPoint(j);
            if (pt.getDistance() < 0.f) {
               	if(objetoA != 0 && objetoB != 0){
					Entidad* a = (Entidad*)(obA->getUserPointer());
					Entidad* b = (Entidad*)(obB->getUserPointer());

					a->Contact(objetoB, b->GetClase());
					b->Contact(objetoA, a->GetClase());
					break;
				}
            }
        }
	}
}

void BulletEngine::DebugDrawWorld(){
	m_dynamicsWorld->debugDrawWorld();
}

void* BulletEngine::Raycast(vector3df S, vector3df E){

	btVector3 Start(S.X, S.Y, S.Z);
	btVector3 End(E.X, E.Y, E.Z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
    //RayCallback.m_collisionFilterMask = FILTER_CAMERA;

    m_dynamicsWorld->rayTest(Start, End, RayCallback);
	
    //GraphicEngine::getInstance()->paintLineDebug(S, E, vector3df(1.0f, 0.0f, 0.0f));

    if(RayCallback.hasHit()) {

        //End = RayCallback.m_hitPointWorld;
        //Normal = RayCallback.m_hitNormalWorld;
		if(RayCallback.m_collisionObject->getUserPointer()!=0){
			return RayCallback.m_collisionObject->getUserPointer();
   		}
    }
    return NULL;
}

void BulletEngine::motorPreTickCallback (btDynamicsWorld *world, btScalar timeStep){
	//std::cout<<"The world just ticked by "<<(float)timeStep<<" seconds\n"<<std::endl;

	BulletEngine *w = static_cast<BulletEngine *>(world->getWorldUserInfo());
	w->motorProcessCallback(timeStep);
}

void BulletEngine::motorProcessCallback(btScalar timeStep){
	btCollisionObjectArray WorldObjects = m_dynamicsWorld->getCollisionObjectArray();
	for (int i = 0; i < WorldObjects.size(); i++) {
		btGhostObject *ghostObject = btGhostObject::upcast(WorldObjects[i]);
               if (!ghostObject) {
                   continue;
               }
		//TYPE 2 = RIGIDBODY; TYPE 4 = GHOSTBODY
		for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++){
			btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(ghostObject->getOverlappingObject(i));

			Entidad* ghost = (Entidad*)(ghostObject->getUserPointer());
			Entidad* OvObj = (Entidad*)(pRigidBody->getUserPointer());

			ghost->Contact(pRigidBody->getUserPointer(), OvObj->GetClase());

		}	
	}
}

BulletEngine::~BulletEngine(){}