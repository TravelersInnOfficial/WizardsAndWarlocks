#include "./Potion.h"

Potion::Potion(vector3df TPosition, vector3df TScale, vector3df TRotation){
	value = 20;
	clase = EENUM_POTION;
	scale = TScale;
	CreatePotion(TPosition, TRotation);
}

Potion::~Potion(){
	if(!picked){
		bt_body->Erase();
    	m_potionNode->Erase();	
	}

    delete bt_body;
    delete m_potionNode;
}

void Potion::CreatePotion(vector3df TPosition, vector3df TRotation){
	picked = false;

	GraphicEngine* engine = GraphicEngine::getInstance();

	vector3df TCenter = vector3df(0,0,0);

	// Create an Irrlicht cube
	m_potionNode = engine->addObjMeshSceneNode("./../assets/modelos/potion.obj");
	m_potionNode->setPosition(TPosition);
	m_potionNode->setScale(scale);
	m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_potionNode) {
		m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_potionNode->setMaterialTexture(0, "./../assets/textures/lifePotion.png");
    }

	//Bullet Physics
	vector3df HalfExtents(scale.X * 0.5f, scale.Y * 0.95, scale.Z * 0.5f);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,1,1,TCenter, C_POTION, potionCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
	//bt_body->SetCollisionFlags("no_contact");
}

void Potion::DeletePotion(){
	bt_body->Erase();
	m_potionNode->Erase();
}

void Potion::Update(){
	if(!picked) UpdatePosShape();
}

void Potion::Drop(vector3df force){
	bt_body->ApplyCentralImpulse(force);
}

void Potion::Interact(Player* p){
	picked = true;
	DeletePotion();
	p->CatchObject(this);
}

void Potion::Use(Player* p){
	p->ChangeHP(value);
}

void Potion::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_potionNode->setPosition(pos);
}