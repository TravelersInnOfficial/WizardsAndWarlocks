#include "Block.h"
#include <GraphicEngine/GraphicEngine.h>

Block::~Block(){
	delete graphBody;
	delete physBody;
}

Block::Block(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string texture){
	GraphicEngine* engine = GraphicEngine::getInstance();
	
	float TMass = 0;
	clase = EENUM_FLOOR;

	// Create an Irrlicht cube
	graphBody = engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
	if (texture == "")
		graphBody->setMaterialTexture(0, "../assets/textures/none.png");
	else 
		graphBody->setMaterialTexture(0, texture);

	graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
	graphBody->setAutomaticCulling();

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