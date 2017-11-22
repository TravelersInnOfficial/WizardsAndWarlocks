#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include "Player.h"
#include "EventReciver.cpp"
#include "PhysicsEngine/BulletEngine.h"
#include "PhysicsEngine/BT_Body.h"
#include "PhysicsEngine/BulletDebug.h"

void createObj(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver *driver,vector3df TPosition, irr::core::vector3df TPosition_irr, irr::core::vector3df TScale, int texture, bool rotate){
	float TMass = 0;

	// Create an Irrlicht cube
	scene::ISceneNode* Node =  sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("./../assets/modelos/pocion.obj"));

	Node->setScale(TScale);
	Node->setPosition(TPosition_irr);
	Node->setMaterialFlag(video::EMF_LIGHTING, false);

	if(texture == 0) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
	else if(texture == 1) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/stones.jpg"));
	else if(texture == 2) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/rockwall.jpg"));
	else if(texture == 3) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/pocion.png"));

	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setAutomaticCulling( irr::scene::EAC_FRUSTUM_BOX  );

	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	BT_Body* RigidBody = new BT_Body();
	RigidBody->CreateBox(TPosition, HalfExtents,TMass,0);

	if(rotate){
		vector3df rot(45,0,0);
		irr::core::vector3df newRot(rot.X,rot.Y,rot.Z);
		Node->setRotation(newRot);
		vector3df rot2(0,45,0);
		RigidBody->Rotate(rot2);
	}
}

void createBox(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver *driver,vector3df TPosition, irr::core::vector3df TPosition_irr, irr::core::vector3df TScale, int texture, bool rotate){
	float TMass = 0;

	// Create an Irrlicht cube
	scene::ISceneNode *Node = sceneManager->addCubeSceneNode(1.0f);

	Node->setScale(TScale);
	Node->setPosition(TPosition_irr);
	Node->setMaterialFlag(video::EMF_LIGHTING, false);

	if(texture == 0) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
	else if(texture == 1) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/stones.jpg"));
	else if(texture == 2) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/rockwall.jpg"));
	else if(texture == 3) Node->setMaterialTexture(0, driver->getTexture("./../assets/textures/pocion.png"));

	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setAutomaticCulling( irr::scene::EAC_FRUSTUM_BOX  );

	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	BT_Body* RigidBody = new BT_Body();
	RigidBody->CreateBox(TPosition, HalfExtents,TMass,0);

	if(rotate){
		vector3df rot(45,0,0);
		irr::core::vector3df newRot(rot.X,rot.Y,rot.Z);
		Node->setRotation(newRot);
		vector3df rot2(0,45,0);
		RigidBody->Rotate(rot2);
	}
}

void createScenery(irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver *driver){
	vector3df TPosition(0,-1,5);
	irr::core::vector3df TPosition_irr(0,-1,5);
	irr::core::vector3df TScale(20,1,20);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 0, false);

	TPosition = vector3df(0,0,3);
	TPosition_irr = irr::core::vector3df(0,0,3);
	TScale = irr::core::vector3df(1,1,1);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = vector3df(3,0.5,3);
	TPosition_irr = irr::core::vector3df(3,0.5,3);
	TScale = irr::core::vector3df(2,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = vector3df(1.5,1,7);
	TPosition_irr = irr::core::vector3df(1.5,1,7);
	TScale = irr::core::vector3df(3,3,3);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = vector3df(5,1.5,7);
	TPosition_irr = irr::core::vector3df(5,1.5,7);
	TScale = irr::core::vector3df(5,5,5);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, false);

	TPosition = vector3df(-5,-1,2.5);
	TPosition_irr = irr::core::vector3df(-5,-1,2.5);
	TScale = irr::core::vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = vector3df(-5,-1,4.5);
	TPosition_irr = irr::core::vector3df(-5,-1,4.5);
	TScale = irr::core::vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = vector3df(-5,-1,6.5);
	TPosition_irr = irr::core::vector3df(-5,-1,6.5);
	TScale = irr::core::vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = vector3df(-5,-1,8.5);
	TPosition_irr = irr::core::vector3df(-5,-1,8.5);
	TScale = irr::core::vector3df(3,2,2);
	createBox(sceneManager, driver, TPosition, TPosition_irr, TScale, 2, true);

	TPosition = vector3df(5,4,7);
	TPosition_irr = irr::core::vector3df(5,4,7);
	TScale = irr::core::vector3df(0.4,0.4,0.4);
	createObj(sceneManager, driver, TPosition, TPosition_irr, TScale, 3, false);
}

bool manageInputs(irr::scene::ISceneManager* sceneManager, Player* physicPlayer, MyEventReceiver receiver){
	bool end = false;
	
	if(receiver.IsKeyDown(irr::KEY_ESCAPE)) end = true;
	if(receiver.IsKeyDown(irr::KEY_SPACE)) physicPlayer->Jump();
	
	if(receiver.IsKeyDown(irr::KEY_KEY_W)) physicPlayer->MoveZ(1, sceneManager);
	else if(receiver.IsKeyDown(irr::KEY_KEY_S)) physicPlayer->MoveZ(-1, sceneManager);
	
	if(receiver.IsKeyDown(irr::KEY_KEY_A)) physicPlayer->MoveX(-1, sceneManager);
	else if(receiver.IsKeyDown(irr::KEY_KEY_D)) physicPlayer->MoveX(1, sceneManager);

	if(receiver.IsKeyDown(irr::KEY_KEY_P)) physicPlayer->ChangeHP(-5);
	else if(receiver.IsKeyDown(irr::KEY_KEY_O)) physicPlayer->ChangeHP(+3);

	if(receiver.IsKeyDown(irr::KEY_KEY_R)) physicPlayer->Respawn(sceneManager);
	
	return end;
}

int main() {
	
	// START PhysicsEngine
	BulletEngine::GetInstance()->CreateWorld();

	// START IRRLICHT
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
	physicPlayer->CreatePlayer(sceneManager, device, driver);

	bool end = false;
	while(device->run() && !end){
		BulletEngine::GetInstance()->UpdateWorld();
		physicPlayer->Update(sceneManager, true);
		end = manageInputs(sceneManager, physicPlayer, receiver);
		driver->beginScene(true, true, irr::video::SColor(255,150,150,255)); // Color de borrado en ARGB
		sceneManager->drawAll();
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
		driver->endScene();
	}

	BulletEngine::GetInstance()->EraseWorld(); // END BULLET
	device->drop(); // END IRRLICHT

	return 0;
}