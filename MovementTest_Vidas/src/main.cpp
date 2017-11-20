#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "./Player.h"
#include "./EventReciver.cpp"
#include <chrono>
#include <thread>

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
}

void Update3DWorld(){
	didneyWorl.dynamicsWorld->stepSimulation(1 / 60.f, 7);
}

void Erase3DWorld(){
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
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 0, false);

	TPosition = btVector3(0,0,3);
	TPosition_irr = vector3df(0,0,3);
	TScale = vector3df(1,1,1);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = btVector3(3,0.5,3);
	TPosition_irr = vector3df(3,0.5,3);
	TScale = vector3df(2,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = btVector3(1.5,1,7);
	TPosition_irr = vector3df(1.5,1,7);
	TScale = vector3df(3,3,3);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = btVector3(5,1.5,7);
	TPosition_irr = vector3df(5,1.5,7);
	TScale = vector3df(5,5,5);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = btVector3(-5,-1,2.5);
	TPosition_irr = vector3df(-5,-1,2.5);
	TScale = vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = btVector3(-5,-1,4.5);
	TPosition_irr = vector3df(-5,-1,4.5);
	TScale = vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = btVector3(-5,-1,6.5);
	TPosition_irr = vector3df(-5,-1,6.5);
	TScale = vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = btVector3(-5,-1,8.5);
	TPosition_irr = vector3df(-5,-1,8.5);
	TScale = vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);
}

bool manageInputs(irr::scene::ISceneManager* sceneManager, Player* physicPlayer, MyEventReceiver receiver){
	bool end = false;
	
	if(receiver.IsKeyDown(irr::KEY_ESCAPE)) end = true;
	if(receiver.IsKeyDown(irr::KEY_SPACE)) physicPlayer->Jump();
	
	if(receiver.IsKeyDown(irr::KEY_KEY_W)) physicPlayer->MoveZ(1, sceneManager);
	else if(receiver.IsKeyDown(irr::KEY_KEY_S)) physicPlayer->MoveZ(-1, sceneManager);
	
	if(receiver.IsKeyDown(irr::KEY_KEY_A)) physicPlayer->MoveX(-1, sceneManager);
	else if(receiver.IsKeyDown(irr::KEY_KEY_D)) physicPlayer->MoveX(1, sceneManager);
	
	/***********LIFE POINTS TEST************/
	if(receiver.IsKeyDown(irr::KEY_KEY_P)) physicPlayer->ChangeHP(-5);
	else if(receiver.IsKeyDown(irr::KEY_KEY_O)) physicPlayer->ChangeHP(+3);

	/***********RESPAWN TEST************/
	if(receiver.IsKeyDown(irr::KEY_KEY_R)) physicPlayer->Respawn();
	
	return end;
}

int main() {

	Create3DWorld(); // START BULLET

	// START IRRLICHT

	// Get screen res
	IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
	core::dimension2d<u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	nulldevice -> drop();

	MyEventReceiver receiver;
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, deskres, 32, true, false, true, &receiver);
	if (!device) return 1;
	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *sceneManager = device->getSceneManager();
	
	device->getCursorControl()->setVisible(false);
	device->setWindowCaption(L"Movement Test");
	sceneManager->addCameraSceneNodeFPS(0, 120, 0);

	createScenery(sceneManager, driver);

	// START JUGADOR
	Player* physicPlayer = new Player();
	physicPlayer->CreatePlayer(didneyWorl.dynamicsWorld, sceneManager, device, driver);

	bool end = false;
	while(device->run() && !end){
		Update3DWorld();
		physicPlayer->Update(sceneManager, true);
		end = manageInputs(sceneManager, physicPlayer, receiver);
		driver->beginScene(true, true, SColor(255,150,150,255)); // Color de borrado en ARGB
		sceneManager->drawAll();
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
		driver->endScene();
	}

	Erase3DWorld(); // END BULLET
	device->drop(); // END IRRLICHT

	return 0;
}