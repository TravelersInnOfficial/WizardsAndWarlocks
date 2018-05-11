#include "./Potion.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/ObjectManager.h"
#include <NetworkEngine/NetworkEngine.h>
#include <GraphicEngine/GBody.h>
#include <GraphicEngine/GSprite.h>
#include <PhysicsEngine/BT_Body.h>
#include <NetworkEngine/Server.h>
#include <GUIEngine/GUIEngine.h>
#include "./../Players/Player.h"
#include <kinematicTypes.h>

Potion::Potion(vector3df TScale, int val, std::string info){	
	clase = EENUM_POTION;
	potionScale = TScale;
	value = val;
	m_info = info;
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

void Potion::ShowInteractInfo(){
	GUIEngine::GetInstance()->ShowEntityInfo(m_info);
}

void Potion::Interact(Player* p){
    NetworkEngine* n_engine = NetworkEngine::GetInstance();

	// If not a client, pick up the potion
	if(!n_engine->IsClientInit()){
		if(!p->HasObject()){
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
}

// ONLY FOR CLIENTS ON NETWORK
void Potion::NetInteract(Player* p){
	if(!p->HasObject()){
		picked = true;
		DeletePotion();
		p->CatchObject(this);
		player = p;
	}
}

void Potion::CreatePotion(vector3df TPosition, vector3df TRotation){
	picked = false;
	player = nullptr;

	GraphicEngine* g_engine = GraphicEngine::getInstance();
	vector3df TCenter = vector3df(0,0,0);

	// Create graphic body loading mesh
	m_potionNode = g_engine->addObjMeshSceneNode(m_potionpath);
	m_potionNode->setPosition(TPosition);
	m_potionNode->setScale(potionScale*1.5);

	//Bullet Physics
	vector3df HalfExtents(potionScale.X*0.1f, potionScale.Y*0.15f, potionScale.Z*0.1f);
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

std::string Potion::GetHUDTexture(){ 
	return HUDTexturePath;
}

void Potion::SetPosition(vector3df pos){
    bt_body->SetPosition(pos);
    m_potionNode->setPosition(pos);
}