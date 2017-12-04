#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include <vector3d.h>

//Controlers
#include "Game.h"
#include "ManagerObject.h"

void createScenery(){
	ManagerObject* objman = ManagerObject::GetInstance(); 

	vector3df TPosition(0,-1,0);
	vector3df TRotation(0,0,0);
	vector3df TScale(20,1,20);
	objman->AddBlock(TPosition, TScale, TRotation, 0);

	TPosition = vector3df(0,0,3);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(1,1,1);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(3,0.5,3);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(2,2,2);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(5,4,3);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(2,2,2);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(1.5,1,7);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(3,3,3);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(5,1.5,7);
	TRotation = vector3df(0,0,0);
	TScale = vector3df(5,5,5);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(-5,-1,2.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(-5,-1,4.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(-5,-1,6.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

	TPosition = vector3df(-5,-1,8.5);
	TRotation = vector3df(45,0,0);
	TScale = vector3df(3,2,2);
	objman->AddBlock(TPosition, TScale, TRotation, 2);

//	TPosition = vector3df(5,4,7);
//	TRotation = vector3df(0,0,0);
//	TScale = vector3df(0.4,0.4,0.4);
//	createObj(TPosition, TRotation, TScale, 3);
}

bool manageInputs(Player* physicPlayer){
	GraphicEngine* engine = GraphicEngine::getInstance();
	bool end = false;
	
	
	if(engine->IsKeyPressed(KEY_ESCAPE)) {
		end = true;
	}
	if(engine->IsLeftButtonPressed()){ 
		ManagerSpell::GetInstance()->ResetHechizo(0,physicPlayer);
		physicPlayer->DropObject();
	}
	if(engine->IsLeftButtonDown()){  ManagerSpell::GetInstance()->LanzarHechizo(0,physicPlayer); 
	}
	if(engine->IsKeyPressed(KEY_KEY_E)){ ManagerSpell::GetInstance()->LanzarHechizo(1,physicPlayer);
	physicPlayer->Raycast(); }

	if(engine->IsKeyPressed(KEY_KEY_Z)){
		physicPlayer->UseObject();
	}

	if(engine->IsKeyDown(KEY_SPACE)){ 
		physicPlayer->Jump(); 
	}
	if(engine->IsKeyDown(KEY_KEY_W)) physicPlayer->MoveZ(1);
	else if(engine->IsKeyDown(KEY_KEY_S)) physicPlayer->MoveZ(-1);
	
	if(engine->IsKeyDown(KEY_KEY_A)) physicPlayer->MoveX(-1);
	else if(engine->IsKeyDown(KEY_KEY_D)) physicPlayer->MoveX(1);

	if(engine->IsKeyPressed(KEY_KEY_P)) physicPlayer->ChangeHP(-5);
	else if(engine->IsKeyPressed(KEY_KEY_O)) physicPlayer->ChangeHP(+3);

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

	//crete level
	createScenery();

	Game* game = Game::GetInstance();
	bool end = false;
	while(g_engine->run() && !end){
		game->UpdateDelta();
		end = game->Input();
		game->Update();
		game->Draw();

		//std::this_thread::sleep_for(std::chrono::milliseconds(7));
		g_engine->endScene();	
	}
	delete game;

	f_engine->EraseWorld(); // END BULLET
	g_engine->drop(); // END IRRLICHT

	return 0;
}