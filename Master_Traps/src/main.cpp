#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include "vector3d.h"
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "Player.h"
#include "Trap.h"

void createObj(vector3df TPosition, vector3df TRotation, vector3df TScale, int texture){
	GraphicEngine* engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create an Irrlicht cube
	GBody* Node = engine->addObjMeshSceneNode("./../assets/modelos/pocion.obj");
	Node->setPosition(TPosition);
	Node->setScale(TScale);
	Node->setMaterialFlag("lighting", false);

	if(texture == 0) Node->setMaterialTexture("./../assets/textures/wall.bmp");
	else if(texture == 1) Node->setMaterialTexture("./../assets/textures/stones.jpg");
	else if(texture == 2) Node->setMaterialTexture("./../assets/textures/rockwall.jpg");
	else if(texture == 3) Node->setMaterialTexture("./../assets/textures/pocion.png");

	Node->setMaterialFlag("normalize", true);
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
	Node->setMaterialFlag("lighting", false);

	if(texture == 0) Node->setMaterialTexture( "./../assets/textures/wall.bmp");
	else if(texture == 1) Node->setMaterialTexture("./../assets/textures/stones.jpg");
	else if(texture == 2) Node->setMaterialTexture("./../assets/textures/rockwall.jpg");
	else if(texture == 3) Node->setMaterialTexture("./../assets/textures/pocion.png");

	Node->setMaterialFlag("normalize", true);
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

	TPosition = vector3df(5,4,7);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(0.4,0.4,0.4);
	createObj(TPosition, TRotation, TScale, 3);

	Trap* trap = new Trap(vector3df(0,0,0),vector3df(1,0,1),TENUM_DEATH_CLAWS);
	Trap* trap2 = new Trap(vector3df(5,0,0),vector3df(1,0,1),TENUM_SPIRITS);
}

bool manageInputs(Player* physicPlayer){
	GraphicEngine* engine = GraphicEngine::getInstance();
	bool end = false;
	
	if(engine->IsKeyDown(KEY_ESCAPE)) end = true;
	if(engine->IsKeyDown(KEY_SPACE)) physicPlayer->Jump();
	
	if(engine->IsKeyDown(KEY_KEY_W)) physicPlayer->MoveZ(1);
	else if(engine->IsKeyDown(KEY_KEY_S)) physicPlayer->MoveZ(-1);
	
	if(engine->IsKeyDown(KEY_KEY_A)) physicPlayer->MoveX(-1);
	else if(engine->IsKeyDown(KEY_KEY_D)) physicPlayer->MoveX(1);

	if(engine->IsKeyDown(KEY_KEY_P)) physicPlayer->ChangeHP(-5);
	else if(engine->IsKeyDown(KEY_KEY_O)) physicPlayer->ChangeHP(+3);

	if(engine->IsKeyDown(KEY_KEY_R)) physicPlayer->Respawn();
	
	return end;
}

int main() {
	
	// START PhysicsEngine
	BulletEngine::GetInstance()->CreateWorld();

	// START IRRLICHT
	GraphicEngine* engine = GraphicEngine::getInstance();
	
	engine->setCursorVisible(false);
	engine->addCameraSceneNodeFPS(120.f, 0.f);

	createScenery();

	// START JUGADOR
	Player* physicPlayer = new Player(true);

	bool end = false;
	while(engine->run() && !end){
		BulletEngine::GetInstance()->UpdateWorld();
		physicPlayer->Update();
		end = manageInputs(physicPlayer);
		engine->beginSceneDefault(); // Color de borrado en ARGB
		engine->drawAll();
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
		engine->endScene();
	}

	BulletEngine::GetInstance()->EraseWorld(); // END BULLET
	engine->drop(); // END IRRLICHT

	return 0;
}