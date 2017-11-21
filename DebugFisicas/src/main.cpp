//Calcular Vector3D a partir de dos angulos
//https://math.stackexchange.com/questions/1385137/calculate-3d-vector-out-of-two-angles-and-vector-length

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include "Entidad.h"
#include "./Player.h"
#include "Bloque.h"
#include "BulletDebug.h"
#include "ControlProyectil.h"
#include "ControlHechizo.h"
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
	btCollisionShape* groundShape;
	btDefaultMotionState* groundMotionState;
    btRigidBody* groundRigidBody;
} didneyWorl;

void Create3DWorld(){
	//NEEDS FOR CREATING THE WORLD
	didneyWorl.broadphase = new btDbvtBroadphase();
	didneyWorl.collisionConfiguration = new btDefaultCollisionConfiguration();       
	didneyWorl.dispatcher = new btCollisionDispatcher(didneyWorl.collisionConfiguration);
	didneyWorl.solver = new btSequentialImpulseConstraintSolver;
	
	//INSTANTIATES THE DYNAMICS WORLD
	didneyWorl.dynamicsWorld = new btDiscreteDynamicsWorld(didneyWorl.dispatcher, didneyWorl.broadphase, didneyWorl.solver, didneyWorl.collisionConfiguration);
	
	//SETS THE GRAVITY
	didneyWorl.dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	
	//CREATES THE GROUND
	didneyWorl.groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	//GROUND INITIAL MOTION STATE
	didneyWorl.groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	//CREATE GROUND INFO
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, didneyWorl.groundMotionState, didneyWorl.groundShape, btVector3(0, 0, 0));

    //AGREGATE THE GROUND INFO TO THE GROUND RIGID BODY
    didneyWorl.groundRigidBody = new btRigidBody(groundRigidBodyCI);

    //ADD THE GROUND TO THE WORLD
    didneyWorl.dynamicsWorld->addRigidBody(didneyWorl.groundRigidBody);
}

void ComprobarColisiones(){
	int numManifolds = didneyWorl.dispatcher->getNumManifolds();
	for(int i=0; i<numManifolds; i++){
		btPersistentManifold* contactManifold = didneyWorl.dispatcher->getManifoldByIndexInternal(i);
		const btCollisionObject* obA = contactManifold->getBody0();
		const btCollisionObject* obB = contactManifold->getBody1();

		int numContacts = contactManifold->getNumContacts();
		for(int j=0; j<numContacts; j++){
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if(pt.getDistance() < 0.f){
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;

				void* objetoA = obA->getUserPointer();
				void* objetoB = obB->getUserPointer();
				if(objetoA != 0 && objetoB != 0){
					Entidad* a = (Entidad*)(obA->getUserPointer());
					Entidad* b = (Entidad*)(obB->getUserPointer());

					a->Contact(objetoB, b->GetClase());
					b->Contact(objetoA, a->GetClase());
				}
			}
		}

	}
}

void Update3DWorld(){
	didneyWorl.dynamicsWorld->stepSimulation(1 / 60.f, 7);
	ComprobarColisiones();
}

void Erase3DWorld(){
  	//Clean up behind ourselves like good little programmers
    didneyWorl.dynamicsWorld->removeRigidBody(didneyWorl.groundRigidBody);
    delete didneyWorl.groundRigidBody->getMotionState();
    delete didneyWorl.groundRigidBody;
    delete didneyWorl.groundShape;
    delete didneyWorl.dynamicsWorld;
    delete didneyWorl.solver;
    delete didneyWorl.collisionConfiguration;
    delete didneyWorl.dispatcher;
    delete didneyWorl.broadphase;
}

vector<Proyectil*> buletin;

int main() {

	// BULLET
	Create3DWorld();

	// IRRLICHT
	MyEventReceiver receiver;
	IrrlichtDevice *device = createDevice( video::EDT_SOFTWARE, dimension2d<u32>(900, 600), 16, false, false, false, &receiver);
	if (!device) return 1;

	device->setWindowCaption(L"Movement Test");
	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *sceneManager = device->getSceneManager();

	const irr::scene::IGeometryCreator *geomentryCreator = sceneManager->getGeometryCreator();
	


	/*irr::scene::IMesh* plane = geomentryCreator->createPlaneMesh(irr::core::dimension2d<irr::f32>(1, 1), irr::core::dimension2d<irr::u32>(100, 100));
	plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	irr::scene::ISceneNode* ground = sceneManager->addMeshSceneNode(plane);
	ground->setPosition(irr::core::vector3df(0, -5, 0));
	ground->setMaterialTexture(0, driver->getTexture("./../assets/textures/stones.jpg"));*/

	BulletDebug* debugFisicas;
	debugFisicas = new BulletDebug;
	debugFisicas->setDriver(driver);
	didneyWorl.dynamicsWorld->setDebugDrawer(debugFisicas);

	// JUGADOR
	Player* physicPlayer = new Player();
	physicPlayer->CreatePlayer(didneyWorl.dynamicsWorld, sceneManager, device, driver);

	ControlProyectil* Protos = ControlProyectil::GetInstance();
	Protos->AddThings(didneyWorl.dynamicsWorld, sceneManager, device, driver);

	while(device->run()){
		Update3DWorld();
		physicPlayer->Update();

		if(receiver.IsKeyDown(irr::KEY_SPACE)){ 
			physicPlayer->Jump();
		}
		if(receiver.IsKeyDown(irr::KEY_KEY_W)){ 
			physicPlayer->MoveZ(1);
			//Protos->AddToDeleteProyecil(Protos->GetProyectil());
		}
		if(receiver.IsKeyDown(irr::KEY_KEY_A)) physicPlayer->MoveX(-1);
		if(receiver.IsKeyDown(irr::KEY_KEY_S)) physicPlayer->MoveZ(-1);
		else if(receiver.IsKeyDown(irr::KEY_KEY_D)) physicPlayer->MoveX(1);
		Protos->DeleteProyectiles();
		Protos->Update();


		driver->beginScene(true, true, SColor(255,200,200,255)); // Color de borrado en ARGB
		sceneManager->drawAll();
		didneyWorl.dynamicsWorld->debugDrawWorld();
		driver->endScene();
	}

	// End BULLET
	Erase3DWorld();

	// End IRRLICHT
	device->drop();

	return 0;
}