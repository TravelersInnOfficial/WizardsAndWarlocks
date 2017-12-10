#include "./Potion.h"

Potion::Potion(vector3df TPosition, vector3df TScale, vector3df TRotation){
	value = 20;
	clase = EENUM_POTION;
	scale = TScale;
	CreatePotion(TPosition, TRotation);
}

Potion::~Potion(){
	if(!cogida){
		bt_body->Erase();
    	m_potionNode->Erase();	
	}

    delete bt_body;
    delete m_potionNode;
}

void Potion::CreatePotion(vector3df TPosition, vector3df TRotation){
	cogida = false;

	GraphicEngine* engine = GraphicEngine::getInstance();

	// Create an Irrlicht cube
	m_potionNode = engine->addObjMeshSceneNode("./../assets/modelos/pocion.obj");
	m_potionNode->setPosition(TPosition);
	m_potionNode->setScale(scale);
	m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_potionNode) {
		m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_potionNode->setMaterialTexture(0, "./../assets/textures/pocion.png");
    }

	//Bullet Physics
	vector3df HalfExtents(scale.X * 0.5f, scale.Y * 0.5f, scale.Z * 0.5f);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,1,0);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
	//bt_body->SetCollisionFlags("no_contact");
}

void Potion::DeletePotion(){
	bt_body->Erase();
	m_potionNode->Erase();
}

void Potion::Update(){
	if(!cogida){
		UpdatePosShape();
	}
}

void Potion::Interact(Player* p){
	cogida = true;
	DeletePotion();
	p->CatchObject(this);
}

void Potion::Use(Player* p){
	p->ChangeHP(value);
	std::cout<<"USING POTION"<<std::endl;
}

void Potion::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_potionNode->setPosition(pos);
}