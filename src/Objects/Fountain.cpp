#include "Fountain.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

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

Fountain::~Fountain(){
	bt_body->Erase();
    m_fountainNode->Erase();	

    delete bt_body;
    delete m_fountainNode;
}

void Fountain::CreateFountain(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create an Irrlicht cube
	m_fountainNode = engine->addObjMeshSceneNode("./../assets/modelos/fountain.obj");
	m_fountainNode->setPosition(TPosition);
	m_fountainNode->setScale(TScale);
	m_fountainNode->setRotation(TRotation);
	m_fountainNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_fountainNode) {
		m_fountainNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_fountainNode->setMaterialTexture(0, "./../assets/textures/marbre5.jpg");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale * 0.5f * vector3df(1.36876, 2.0, 1.2));
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,TMass,0, vector3df(0,0,0), C_FOUNTAIN, fountainCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

void Fountain::Update(float deltaTime){
	UpdatePosShape();
	currentTime += deltaTime;

	if(currentTime >= maxTime){
		if(inUse){
			Use();
		}else{
			Recover();
			SetFree();
		} 
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
	if(user!=NULL){
		if(incrementUse<=value){		
			value -= incrementUse;
			user->ChangeHP(incrementUse);
			user->ChangeMP(incrementUse);
			return true;
		}
	}
	return false;
}

bool Fountain::GetInUse(){
	return inUse;
}

float Fountain::GetPercentualValue(){
	float output = value/(float)maxValue;
	return output;
}

void Fountain::Recover(){
	value += incrementValue;
	if(value > maxValue){
		value = maxValue;
	}
}

void Fountain::Interact(Player* p){
	if(user==NULL){
		user = p;
	}
	inUse = true;
}

void Fountain::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_fountainNode->setPosition(pos);
}


void Fountain::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, this, true, AI_FOUNTAIN, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Fountain::GetKinematic(){
	Kinematic cKin;
	cKin.position = bt_body->GetPosition();
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}