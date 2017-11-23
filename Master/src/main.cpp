#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include "vector3d.h"
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"

#include "Player.h"
#include "ControlHechizo.h"
#include "ControlProyectil.h"

void DeleteMain(){
	ControlProyectil::GetInstance()->DeleteAllProyectiles();
}

void createObj(vector3df TPosition, vector3df TRotation, vector3df TScale, int texture){
	GraphicEngine* engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create an Irrlicht cube
	GBody* Node = engine->addObjMeshSceneNode("./../assets/modelos/pocion.obj");
	Node->setPosition(TPosition);
	Node->setScale(TScale);
	Node->setMaterialFlag("lighting", false);

	if(texture == 0) Node->setMaterialTexture(0, "./../assets/textures/wall.bmp");
	else if(texture == 1) Node->setMaterialTexture(0, "./../assets/textures/stones.jpg");
	else if(texture == 2) Node->setMaterialTexture(0, "./../assets/textures/rockwall.jpg");
	else if(texture == 3) Node->setMaterialTexture(0, "./../assets/textures/pocion.png");

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

	if(texture == 0) Node->setMaterialTexture(0, "./../assets/textures/wall.bmp");
	else if(texture == 1) Node->setMaterialTexture(0, "./../assets/textures/stones.jpg");
	else if(texture == 2) Node->setMaterialTexture(0, "./../assets/textures/rockwall.jpg");
	else if(texture == 3) Node->setMaterialTexture(0, "./../assets/textures/pocion.png");

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
}

bool manageInputs(Player* physicPlayer){
	GraphicEngine* engine = GraphicEngine::getInstance();
	bool end = false;
	
	if(engine->IsKeyDown(KEY_ESCAPE)) {
		DeleteMain();
		end = true;
	}
	if(engine->IsKeyDown(KEY_SPACE)){ 
		physicPlayer->Jump();
		ControlHechizo::GetInstance()->LanzarHechizo(0,physicPlayer);
	}
	
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
	BulletEngine* f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();

	// START IRRLICHT
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	g_engine->setCursorVisible(false);
	g_engine->addCameraSceneNodeFPS(120.f, 0.f);

	createScenery();

	// START CONTROLHECHIZO
	ControlHechizo* masterSpell = ControlHechizo::GetInstance();

	// START CONTROLPROYECTIL
	ControlProyectil* masterBullet = ControlProyectil::GetInstance();

	// START JUGADOR
	Player* physicPlayer = new Player(true);
	masterSpell->AddHechizo(0, physicPlayer);

	bool end = false;
	while(g_engine->run() && !end){
		f_engine->UpdateWorld();
		physicPlayer->Update();
		masterBullet->Update();
		end = manageInputs(physicPlayer);
		g_engine->beginSceneDefault(); // Color de borrado en ARGB

		g_engine->drawAll();
		f_engine->DebugDrawWorld();
		std::this_thread::sleep_for(std::chrono::milliseconds(7));
		g_engine->endScene();
	}

	f_engine->EraseWorld(); // END BULLET
	g_engine->drop(); // END IRRLICHT

	return 0;
}