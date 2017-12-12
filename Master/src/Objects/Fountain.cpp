#include "Fountain.h"

Fountain::Fountain(vector3df TPosition, vector3df TScale, vector3df TRotation){
	inUse = false;
	value = 100;
	maxValue = 100;
	incrementUse = 5;
	incrementValue = 2;
	

	maxTime = 0.5f;
	currentTime = 0.0f;

	 clase = EENUM_FOUNTAIN;   
	CreateFountain(TPosition, TScale, TRotation);
}

void Fountain::CreateFountain(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create an Irrlicht cube
	m_fountainNode = engine->addObjMeshSceneNode("./../assets/modelos/fuente.obj");
	m_fountainNode->setPosition(TPosition);
	m_fountainNode->setScale(TScale);
	m_fountainNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_fountainNode) {
		m_fountainNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_fountainNode->setMaterialTexture(0, "./../assets/textures/pocion.png");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,TMass,0, vector3df(0,0,0), C_FOUNTAIN, fountainCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

void Fountain::Update(float deltaTime){
	UpdatePosShape();
	currentTime += deltaTime;

	if(currentTime >= maxTime){
		if(inUse) Use();
		else Recover();
		currentTime = 0.0f;
	}
	
	inUse = false;
}

void Fountain::Update(){
	std::cout<<"NO USAR ESTE UPDATE"<<std::endl;
}

void Fountain::SetFree(){
	user = NULL;
	inUse = false;
}

bool Fountain::Use(){
	if(user){
		if(incrementUse<=value){		
			value -= incrementUse;
			user->ChangeHP(incrementUse);
			return true;
		}
	}
	return false;
}

void Fountain::Recover(){
	value += incrementValue;
	if(value > maxValue){
		value = maxValue;
	}
}

void Fountain::Interact(Player* p){
	inUse = true;
	user = p;
}

void Fountain::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_fountainNode->setPosition(pos);
}