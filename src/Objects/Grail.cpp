#include "Grail.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

Grail::Grail(vector3df TPosition, vector3df TScale, vector3df TRotation){
	casting = false;
	recovered = false;
	clase = EENUM_GRAIL;

	timeCasting = 0.0f;
	maxCasting = 5.0f;
	createSoundEvent();
	CreateGrail(TPosition, TScale, TRotation);
}

void Grail::CreateGrail(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	// Create graphic body loading mesh
	m_grailNode = g_engine->addObjMeshSceneNode("./../assets/modelos/grail.obj", TPosition, TRotation, TScale);
	m_grailNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_grailNode) {
		m_grailNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        //m_grailNode->setMaterialTexture(0, "./../assets/textures/gold.jpg");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.7, TScale.Y*0.7, TScale.Z*0.7);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,0,0,vector3df(0,0,0), C_GRAIL, grailCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
	
	playEvent(TPosition);
}

Grail::~Grail(){
	soundEvent->release();
	delete soundEvent;

    delete bt_body;
    delete m_grailNode;
}

void Grail::Update(float deltaTime){
	
	if(casting){
		timeCasting += deltaTime; 
		casting = false; 
	}
	else{
		playerOneInteraction = false;
		timeCasting = 0.0f;
	}
	
	UpdatePosShape();
}

void Grail::Update(){
	std::cout<<"NO USAR ESTE UPDATE"<<std::endl;
}

void Grail::Interact(Player* p){
	if(p->GetAlliance() == ALLIANCE_WIZARD){

		if(timeCasting >= maxCasting){
			recovered = true;
			timeCasting = 0.0f;
		}

		casting = true;
		if(p->IsPlayerOne()) playerOneInteraction = true;
	}
}

void Grail::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_grailNode->setPosition(pos);
}

bool Grail::CheckIfWon(){
	bool toRet = recovered;
	if(recovered) recovered = false;
	return toRet;
}

void Grail::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, this, true, AI_GRAIL, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Grail::GetKinematic(){
	Kinematic cKin;
	cKin.position = bt_body->GetPosition();
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

void Grail::drawGUI(){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	if(timeCasting > 0){
		if(playerOneInteraction || n_engine->IsServerInit()){
			GraphicEngine* g_engine = GraphicEngine::getInstance();
			if(g_engine != nullptr) g_engine->drawGrailGUI(timeCasting, maxCasting);
		}
	}
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void Grail::createSoundEvent() {
	soundEvent = SoundSystem::getInstance()->createEvent("event:/CommonSounds/Grail/Levitation");
}

void Grail::playEvent(vector3df pos) {
	SoundSystem::getInstance()->playEvent(soundEvent, pos);
}