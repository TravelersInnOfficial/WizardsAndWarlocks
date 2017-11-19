#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "./Player.h"
#include "./EventReciver.cpp"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct World{
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;      
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	//btCollisionShape* groundShape;
	//btDefaultMotionState* groundMotionState;
    //btRigidBody* groundRigidBody;
} didneyWorl;

void Create3DWorld(){
	//NEEDS FOR CREATING THE WORLD
	didneyWorl.broadphase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
	didneyWorl.collisionConfiguration = new btDefaultCollisionConfiguration();       
	didneyWorl.dispatcher = new btCollisionDispatcher(didneyWorl.collisionConfiguration);
	didneyWorl.solver = new btSequentialImpulseConstraintSolver;
	
	//INSTANTIATES THE DYNAMICS WORLD
	didneyWorl.dynamicsWorld = new btDiscreteDynamicsWorld(didneyWorl.dispatcher, didneyWorl.broadphase, didneyWorl.solver, didneyWorl.collisionConfiguration);
	
	//SETS THE GRAVITY
	didneyWorl.dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	
	//CREATES THE GROUND
	//didneyWorl.groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	//GROUND INITIAL MOTION STATE
	//didneyWorl.groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -10, 0)));

	//CREATE GROUND INFO
    //btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, didneyWorl.groundMotionState, didneyWorl.groundShape, btVector3(0, 0, 0));
	//groundRigidBodyCI.m_friction = 0.8;

    //AGREGATE THE GROUND INFO TO THE GROUND RIGID BODY
    //didneyWorl.groundRigidBody = new btRigidBody(groundRigidBodyCI);

    //ADD THE GROUND TO THE WORLD
    //didneyWorl.dynamicsWorld->addRigidBody(didneyWorl.groundRigidBody);

}

void Update3DWorld(){
	didneyWorl.dynamicsWorld->stepSimulation(1 / 60.f, 7);
}

void Erase3DWorld(){
  	//Clean up behind ourselves like good little programmers
    //didneyWorl.dynamicsWorld->removeRigidBody(didneyWorl.groundRigidBody);
    //delete didneyWorl.groundRigidBody->getMotionState();
    //delete didneyWorl.groundRigidBody;
    //delete didneyWorl.groundShape;
    delete didneyWorl.dynamicsWorld;
    delete didneyWorl.solver;
    delete didneyWorl.collisionConfiguration;
    delete didneyWorl.dispatcher;
    delete didneyWorl.broadphase;
}

void createBox(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver *driver,btVector3 TPosition, vector3df TPosition_irr, vector3df TScale, int texture, bool rotate){

	btScalar TMass = 0;

	// Create an Irrlicht cube
	scene::ISceneNode *Node = sceneManager->addCubeSceneNode(1.0f);

	Node->setScale(TScale);
	Node->setPosition(TPosition_irr);
	Node->setMaterialFlag(video::EMF_LIGHTING, false);

	if(texture == 0) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
	else if(texture == 1) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/stones.jpg"));
	else if(texture == 2) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/rockwall.jpg"));

	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setAutomaticCulling( EAC_FRUSTUM_BOX  );
	//Node->setDebugDataVisible(irr::scene::EDS_BBOX);
	//Node->setMaterialTexture(0, irrDriver->getTexture("rust0.jpg"));

	// Set the initial position of the object
	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(TPosition);
	

	// Give it a default MotionState
	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	// Create the shape
	btVector3 HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	btCollisionShape *Shape = new btBoxShape(HalfExtents);

	// Add mass
	btVector3 LocalInertia;
	Shape->calculateLocalInertia(TMass, LocalInertia);

	// Create the rigid body object
	btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(TMass, MotionState, Shape, LocalInertia);
	playerRigidBodyCI.m_friction = 0;
	btRigidBody *RigidBody = new btRigidBody(playerRigidBodyCI);

	// Store a pointer to the irrlicht node so we can update it later
	RigidBody->setUserPointer((void *)(Node));

	if(rotate){
		vector3df newRot(45,0,0);
		Node->setRotation(newRot);

		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEuler(0,45,0);
		tr.setRotation(quat);
		tr.setOrigin(TPosition);
		RigidBody->setCenterOfMassTransform(tr);
	}

	// Add it to the world
	didneyWorl.dynamicsWorld->addRigidBody(RigidBody);
}

void createScenery(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver *driver){
	btVector3 TPosition(0,-1,5);
	vector3df TPosition_irr(0,-1,5);
	vector3df TScale(20,1,20);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 3, false);

	TPosition = btVector3(0,0,3);
	TPosition_irr = vector3df(0,0,3);
	TScale = vector3df(1,1,1);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 1, false);

	TPosition = btVector3(3,0.5,3);
	TPosition_irr = vector3df(3,0.5,3);
	TScale = vector3df(2,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 1, false);

	TPosition = btVector3(1.5,1,7);
	TPosition_irr = vector3df(1.5,1,7);
	TScale = vector3df(3,3,3);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 1, false);

	TPosition = btVector3(5,1.5,7);
	TPosition_irr = vector3df(5,1.5,7);
	TScale = vector3df(5,5,5);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 1, false);


	TPosition = btVector3(-2,-1,2.5);
	TPosition_irr = vector3df(-2,-1,2.5);
	TScale = vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 1, true);
}

int main() {

	// BULLET
	Create3DWorld();

	// IRRLICHT
	MyEventReceiver receiver;
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, dimension2d<u32>(900, 600), 16, false, false, false, &receiver);
	if (!device) return 1;
	device->getCursorControl()->setVisible(false);

	device->setWindowCaption(L"Movement Test");
	irr::video::IVideoDriver *driver = device->getVideoDriver(); //video::EDT_OPENGL;
	irr::scene::ISceneManager *sceneManager = device->getSceneManager();

	createScenery(sceneManager, driver);

	// JUGADOR
	Player* physicPlayer = new Player();
	physicPlayer->CreatePlayer(didneyWorl.dynamicsWorld, sceneManager, device, driver);

	/*Player* physicPlayer2 = new Player();
	physicPlayer2->CreatePlayer(didneyWorl.dynamicsWorld, sceneManager, device, driver);
	physicPlayer2->SetPosX(60);*/

	while(device->run()){
		Update3DWorld();
		physicPlayer->Update(sceneManager, true);
		//physicPlayer2->Update(sceneManager, false);

		if(receiver.IsKeyDown(irr::KEY_SPACE)) physicPlayer->Jump();
		if(receiver.IsKeyDown(irr::KEY_KEY_W)) physicPlayer->MoveZ(1, sceneManager);
		if(receiver.IsKeyDown(irr::KEY_KEY_A)) physicPlayer->MoveX(-1, sceneManager);
		if(receiver.IsKeyDown(irr::KEY_KEY_S)) physicPlayer->MoveZ(-1, sceneManager);
		else if(receiver.IsKeyDown(irr::KEY_KEY_D)) physicPlayer->MoveX(1, sceneManager);

		driver->beginScene(true, true, SColor(255,200,200,255)); // Color de borrado en ARGB
		sceneManager->drawAll();
		driver->endScene();
	}

	// End BULLET
	Erase3DWorld();

	// End IRRLICHT
	device->drop();

	return 0;
}