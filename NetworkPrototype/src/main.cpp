#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include "vector3d.h"
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "NetworkEngine/NetworkEngine.h"
#include "Game.h"

void createObj(vector3df TPosition, vector3df TRotation, vector3df TScale, int texture){
	GraphicEngine* engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create an Irrlicht cube
	GBody* Node = engine->addObjMeshSceneNode("./../assets/modelos/pocion.obj");
	Node->setPosition(TPosition);
	Node->setScale(TScale);
	Node->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if(texture == 0) Node->setMaterialTexture(0, "./../assets/textures/wall.bmp");
	else if(texture == 1) Node->setMaterialTexture(0, "./../assets/textures/stones.jpg");
	else if(texture == 2) Node->setMaterialTexture(0, "./../assets/textures/rockwall.jpg");
	else if(texture == 3) Node->setMaterialTexture(0, "./../assets/textures/pocion.png");

	Node->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
	Node->setAutomaticCulling();

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	BT_Body* RigidBody = new BT_Body();
	RigidBody->CreateBox(TPosition, HalfExtents,TMass,0);
	RigidBody->Rotate(TRotation);
}

void createBox(vector3df TPosition, vector3df TRotation, vector3df TScale, int texture){
	GraphicEngine* engine = GraphicEngine::getInstance();
	
	float TMass = 0;

	// Create an Irrlicht cube
	GBody *Node = engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	Node->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if(texture == 0) Node->setMaterialTexture(0, "./../assets/textures/wall.bmp");
	else if(texture == 1) Node->setMaterialTexture(0, "./../assets/textures/stones.jpg");
	else if(texture == 2) Node->setMaterialTexture(0, "./../assets/textures/rockwall.jpg");
	else if(texture == 3) Node->setMaterialTexture(0, "./../assets/textures/pocion.png");

	Node->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
	Node->setAutomaticCulling();

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	BT_Body* RigidBody = new BT_Body();
	RigidBody->CreateBox(TPosition, HalfExtents,TMass,0);
	RigidBody->Rotate(TRotation);
}

void createScenery(){
	vector3df TPosition(0,-1,5);
	vector3df TRotation(0,0,0);
	vector3df TScale(20,1,20);
	createBox(TPosition, TRotation, TScale, 0);

	TPosition = vector3df(0,0,3);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(1,1,1);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(3,0.5,3);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(2,2,2);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(5,4,3);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(2,2,2);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(1.5,1,7);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(3,3,3);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(5,1.5,7);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(5,5,5);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(-5,-1,2.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(-5,-1,4.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(-5,-1,6.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	createBox(TPosition, TRotation, TScale, 2);

	TPosition = vector3df(-5,-1,8.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	createBox(TPosition, TRotation, TScale, 2);

	/*TPosition = vector3df(5,4,7);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(0.4,0.4,0.4);
	createObj(TPosition, TRotation, TScale, 3);*/
}

int main(int argc, char* argv[]) {

	// NETWORK #################################################################
	NetworkEngine* networkEngine = NetworkEngine::GetInstance();
	bool isServer = false;
	if(argc > 1 && strcmp(argv[1],"-s") == 0) { isServer = true; std::cout<<"STARTING SERVER"<<std::endl;}
	if (isServer) networkEngine->StartServer();
	else networkEngine->StartClient();
	// NETWORK #################################################################

	// START PhysicsEngine
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();

	// START IRRLICHT
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	g_engine->setCursorVisible(false);
	if (!isServer) g_engine->addCameraSceneNodeFPS(120.f, 0.f);
	else g_engine->addCameraSceneNodeFPS(120.f, .005f);

	createScenery();

	Game* game = Game::GetInstance(isServer);
	bool end = false;
	while(g_engine->run() && !end){

		// NETWORK #################################################################
		if (isServer) networkEngine->GetServer()->RecievePackages();
		else networkEngine->GetClient()->RecievePackages();
		// NETWORK #################################################################

		game->UpdateDelta();
		end = game->Input();
		game->Update();
		game->Draw();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		g_engine->endScene();	
	}
	delete game;

	f_engine->EraseWorld(); // END BULLET
	g_engine->drop(); // END IRRLICHT

	return 0;
}