#include "./Potion.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../NetworkEngine/Client.h"
#include "./../NetworkEngine/Server.h"

#define MODEL_SIZE 0.136f

Potion::Potion(vector3df TScale, int val, std::string tex){
	clase = EENUM_POTION;
	potionScale = TScale;
	potionTexture = tex;
	value = val;
}

Potion::~Potion(){
	if(!picked){
		bt_body->Erase();
    	m_potionNode->Erase();	
	}

    delete bt_body;
    delete m_potionNode;
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

		// If a server, send the potion signal
		if (n_engine->IsServerInit()){
			Server* server = n_engine->GetServer();
			if(server != NULL){
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
}

void Potion::CreatePotion(vector3df TPosition, vector3df TRotation){
	picked = false;

	GraphicEngine* engine = GraphicEngine::getInstance();

	vector3df TCenter = vector3df(0,0,0);

	// Create an Irrlicht cube
	m_potionNode = engine->addObjMeshSceneNode("./../assets/modelos/potion.obj");
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
	bt_body->Erase();
	m_potionNode->Erase();
}

void Potion::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
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
	cKin.position = bt_body->GetPosition();
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

int Potion::GetValue(){
	return value;
}

void Potion::DrawHUD(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	float W =			g_engine->GetScreenWidth();
	float H =			g_engine->GetScreenHeight();
	float size =		W * 0.075;
	float xInit =		W * 0.87;
	float yInit =		H * 0.05;
	float outline =		5;

	g_engine->draw2DRectangle(vector3df(0,0,0), 
								xInit, 
								yInit, 
								xInit + size, 
								yInit + size);

	vector4df sizeImage(xInit+outline, yInit+outline, xInit+size-outline, yInit+size-outline);
	g_engine->draw2DImage(HUDTexturePath, sizeImage);
}