#include "Block.h"
#include "../GraphicEngine/GraphicEngine.h"

Block::Block(){
	
}

Block::~Block(){
	physBody->Erase();
    graphBody->Erase();	

	delete graphBody;
	delete physBody;
}

Block::Block(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string texture){
	GraphicEngine* engine = GraphicEngine::getInstance();
	
	float TMass = 0;

	// Create an Irrlicht cube
	graphBody = engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if(texture == "") graphBody->setMaterialTexture(0, "./../assets/textures/wall.bmp");
	else graphBody->setMaterialTexture(0, texture);

	graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
	graphBody->setAutomaticCulling();

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	physBody = new BT_Body();
	physBody->CreateBox(TPosition, HalfExtents,TMass,0);
	physBody->Rotate(TRotation);
}

void Block::Update(){
	UpdatePosShape();
}

void Block::UpdatePosShape(){
	physBody->Update();
    vector3df pos = physBody->GetPosition();
    graphBody->setPosition(pos);
}
