#include "./Potion.h"

Potion::Potion(){

	value = 20;

	CreatePotion();
}

void Potion::CreatePotion(){
	vector3df TPosition(-2,0,-2);
	vector3df TScale(1,1,1);
	vector3df TRotation(0,0,0);


	GraphicEngine* engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create an Irrlicht cube
	m_potionNode = engine->addObjMeshSceneNode("./../assets/modelos/pocion.obj");
	m_potionNode->setPosition(TPosition);
	m_potionNode->setScale(TScale);
	m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_potionNode) {
		m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_potionNode->setMaterialTexture(0, "./../assets/textures/pocion.png");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,TMass,0);
	bt_body->Rotate(TRotation);
}

void Potion::Update(){
	UpdatePosShape();
}

void Potion::Interact(Player* p){
	// Coger la pocion
}

void Potion::Use(Player* p){
	p->ChangeHP(value);
	// Eliminar la pocion
}

void Potion::UpdatePosShape(){
	bt_body->Update();
    vector3df* pos = bt_body->GetPosition();
    m_potionNode->setPosition(*pos);
}