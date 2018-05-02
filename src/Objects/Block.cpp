#include "Block.h"
#include <GraphicEngine/GraphicEngine.h>

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
	if (texture == "") graphBody->setMaterialTexture(0, "../assets/textures/none.png");
	else graphBody->setMaterialTexture(0, texture);

	graphBody->setAutomaticCulling();


	if(TScale.X >= TScale.Z && TScale.Y >= TScale.Z) graphBody->SetTextureScale(vector2df(TScale.X/2, TScale.Y/2));
	if(TScale.Z >= TScale.X && TScale.Y >= TScale.X) graphBody->SetTextureScale(vector2df(TScale.Z/2, TScale.Y/2));
	if(TScale.X >= TScale.Y && TScale.Z >= TScale.Y) graphBody->SetTextureScale(vector2df(TScale.X/2, TScale.Z/2));

	if(texture == "") graphBody->SetInvisible();

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