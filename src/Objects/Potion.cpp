#include "./Potion.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/ObjectManager.h"
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/Client.h>
#include <NetworkEngine/Server.h>

#define MODEL_SIZE 0.136f

Potion::Potion(vector3df TScale, int val, std::string tex){
	clase = EENUM_POTION;
	potionScale = TScale;
	potionTexture = tex;
	value = val;
	player = nullptr;
	picked = false;

	m_rect = nullptr;
	m_sprite = nullptr;
}

Potion::~Potion(){
	if(bt_body != nullptr) delete bt_body;
	bt_body = nullptr;
	if(m_potionNode != nullptr) delete m_potionNode;
	m_potionNode = nullptr;
	if(m_sprite != nullptr) delete m_sprite;
	m_sprite = nullptr;
}

bool Potion::CheckUse(Player* p){
	return true;
}

bool Potion::CheckUseHP(Player* p){
	float playerLife = p->GetHP();
	if(playerLife + value <= 100){
		return true;
	}
	return false;
}

bool Potion::CheckUseMP(Player* p){
	float playerMana = p->GetMP();
	if(playerMana + value <= 100){
		return true;
	}
	return false;
}

void Potion::Update(){
	if(!picked) UpdatePosShape();
}

void Potion::Interact(Player* p){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();

	// If not a client, pick up the potion
	if(!n_engine->IsClientInit()){
		picked = true;
		DeletePotion();
		p->CatchObject(this);
		player = p;

		// If a server, send the potion signal
		if (n_engine->IsServerInit()){
			Server* server = n_engine->GetServer();
			if(server != nullptr){
				int pos = ObjectManager::GetInstance()->GetPotionVecPos(this);
				server->NotifyPotionInteracted(pos, p);
			}
		}

	}
}

// ONLY FOR CLIENTS ON NETWORK
void Potion::NetInteract(Player* p){
	picked = true;
	DeletePotion();
	p->CatchObject(this);
	player = p;
}

void Potion::CreatePotion(vector3df TPosition, vector3df TRotation){
	picked = false;
	player = nullptr;

	GraphicEngine* g_engine = GraphicEngine::getInstance();

	vector3df TCenter = vector3df(0,0,0);

	// Create graphic body loading mesh
	m_potionNode = g_engine->addObjMeshSceneNode("./../assets/modelos/potion.obj");
	m_potionNode->setPosition(TPosition);
	m_potionNode->setScale(potionScale);
	m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_potionNode) {
		m_potionNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_potionNode->setMaterialTexture(0, potionTexture);
    }

	//Bullet Physics
	vector3df HalfExtents(potionScale.X * MODEL_SIZE, potionScale.Y * MODEL_SIZE * 1.5, potionScale.Z * MODEL_SIZE);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,1,1,TCenter, C_POTION, potionCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

void Potion::Drop(vector3df force){
	bt_body->ApplyCentralImpulse(force);
}

void Potion::DeletePotion(){
	if(bt_body != nullptr){
		delete bt_body;
		bt_body = nullptr;
	}
	if(m_potionNode != nullptr){
		delete m_potionNode;
		m_potionNode = nullptr;
	}
}

void Potion::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = GetPosition();
    m_potionNode->setPosition(pos);
}

void Potion::SendSignal(){
	if(!picked){
		RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
		// id, AI_code name, float str, Kinematic kin, AI_modalities mod
		sense->AddSignal(id, this, true, AI_POTION, 5.0f, GetKinematic(), AI_SIGHT);
	}
}

Kinematic Potion::GetKinematic(){
	Kinematic cKin;
	cKin.position = GetPosition();
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

int Potion::GetValue(){
	return value;
}

bool Potion::GetPickedState(){
	return picked;
}

Player* Potion::GetUser(){
	return player;
}

vector3df Potion::GetPosition(){
	vector3df toRet = vector3df(0, 0, 0);
	if(bt_body) toRet = bt_body->GetPosition();
	return(toRet);
}

void Potion::EraseHUD(){
	delete m_rect;
	m_rect = nullptr;

	delete m_sprite;
	m_sprite = nullptr;
}

void Potion::DrawHUD(){

	if(m_rect != nullptr){

	}
	else{
		GraphicEngine* g_engine = GraphicEngine::getInstance();

		float W =			g_engine->GetScreenWidth();
		float H =			g_engine->GetScreenHeight();
		float size =		W * 0.075;
		float xInit =		W * 0.87;
		float yInit =		H - size*2;
		float outline =		5;

		vector4df sizeImage(xInit+outline, yInit+outline, xInit+size-outline, yInit+size-outline);

		m_rect = GraphicEngine::getInstance()->add2DRect(vector2df(xInit,yInit), vector2df(size,size));
		
		m_sprite = GraphicEngine::getInstance()->addSprite(HUDTexturePath, vector2df(sizeImage.X,sizeImage.Y), vector2df(size-outline,size-outline));
	}
}

void Potion::SetPosition(vector3df pos){
    bt_body->SetPosition(pos);
    m_potionNode->setPosition(pos);
}