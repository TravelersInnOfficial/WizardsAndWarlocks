#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "./Player.h"
#include "./GraphicsEngine/EventReciver.cpp"

#include "./PhysicsEngine/BulletEngine.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main() {

	// BULLET
	//Create3DWorld();
	BulletEngine::GetInstance()->CreateWorld();

	// IRRLICHT
	MyEventReceiver receiver;
	IrrlichtDevice *device = createDevice( video::EDT_SOFTWARE, dimension2d<u32>(900, 600), 16, false, false, false, &receiver);
	if (!device) return 1;

	device->setWindowCaption(L"Movement Test");
	irr::video::IVideoDriver *driver = device->getVideoDriver();
	irr::scene::ISceneManager *sceneManager = device->getSceneManager();

	//const irr::scene::IGeometryCreator *geomentryCreator = sceneManager->getGeometryCreator();
	/*irr::scene::IMesh* plane = geomentryCreator->createPlaneMesh(irr::core::dimension2d<irr::f32>(1, 1), irr::core::dimension2d<irr::u32>(100, 100));
	plane->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	irr::scene::ISceneNode* ground = sceneManager->addMeshSceneNode(plane);
	ground->setPosition(irr::core::vector3df(0, -5, 0));
	ground->setMaterialTexture(0, driver->getTexture("./../assets/textures/stones.jpg"));*/

	// JUGADOR
	Player* physicPlayer = new Player();
	physicPlayer->CreatePlayer(sceneManager, device, driver);

	while(device->run()){
		//Update3DWorld();
		BulletEngine::GetInstance()->UpdateWorld();

		physicPlayer->Update(sceneManager);

		if(receiver.IsKeyDown(irr::KEY_SPACE)) physicPlayer->Jump();
		if(receiver.IsKeyDown(irr::KEY_KEY_W)) physicPlayer->MoveZ(1);
		if(receiver.IsKeyDown(irr::KEY_KEY_A)) physicPlayer->MoveX(-1);
		if(receiver.IsKeyDown(irr::KEY_KEY_S)) physicPlayer->MoveZ(-1);
		else if(receiver.IsKeyDown(irr::KEY_KEY_D)) physicPlayer->MoveX(1);

		driver->beginScene(true, true, SColor(255,200,200,255)); // Color de borrado en ARGB
		sceneManager->drawAll();
		driver->endScene();
	}

	// End BULLET
	//Erase3DWorld();
	BulletEngine::GetInstance()->EraseWorld();

	// End IRRLICHT
	device->drop();

	return 0;
}