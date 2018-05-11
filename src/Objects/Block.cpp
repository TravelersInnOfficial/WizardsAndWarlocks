#include "Block.h"

#include <GraphicEngine/GraphicEngine.h>
#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GBody.h>

Block::~Block(){
	delete graphBody;
	delete physBody;
}

Block::Block(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string texture){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	
	float TMass = 0;
	clase = EENUM_FLOOR;

	// Create graphic body block
	graphBody = g_engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	graphBody->setAutomaticCulling();


	if(TScale.X >= TScale.Z && TScale.Y >= TScale.Z) graphBody->SetTextureScale(vector2df(TScale.X/2, TScale.Y/2));
	if(TScale.Z >= TScale.X && TScale.Y >= TScale.X) graphBody->SetTextureScale(vector2df(TScale.Z/2, TScale.Y/2));
	if(TScale.X >= TScale.Y && TScale.Z >= TScale.Y) graphBody->SetTextureScale(vector2df(TScale.X/2, TScale.Z/2));

	// Sacamos el bump map

	std::size_t found = texture.find_last_of("/");

  	if(found <= texture.size()){
  		std::string firstPath = texture.substr(0,found);
  		std::string fileName  = texture.substr(found+1);

  		found = firstPath.find_last_of("/");
  		std::string folder = firstPath.substr(found+1);

  		if(found<=firstPath.size()){
  			if(folder.compare("maptextures")==0){
				std::string finalPath = firstPath + "/normal_" + fileName;	

				graphBody->SetBumpMap(finalPath);
  			}
  		}
  	}	

	if(texture == "") graphBody->SetInvisible();
	else graphBody->setMaterialTexture(0, texture);

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	physBody = new BT_Body();
	physBody->CreateBox(TPosition, HalfExtents,TMass,0, vector3df(0,0,0), C_WALL, wallCW);
	physBody->Rotate(TRotation);
	physBody->AssignPointer(this);
}

void Block::Update(){
	UpdatePosShape();
}

void Block::UpdatePosShape(){
	physBody->Update();
    vector3df pos = physBody->GetPosition();
    graphBody->setPosition(pos);
}

vector3df Block::GetPosition(){
	return physBody->GetPosition();
}