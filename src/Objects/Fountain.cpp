#include "Fountain.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"
#include <GUIEngine/GUIEngine.h>

Fountain::Fountain(vector3df TPosition, vector3df TScale, vector3df TRotation){
	user = nullptr;
	inUse = false;
	value = 100;
	maxValue = 100;
	incrementUse = 5;
	incrementValue = 2;

	// La guardamos en radianoes
	rotation = TRotation;
	rotation.toRadians();

	maxTime = 0.5f;
	currentTime = 0.0f;

	clase = EENUM_FOUNTAIN;   
	CreateFountain(TPosition, TScale, TRotation);
	createSoundEvent();
}

Fountain::~Fountain(){
    delete bt_body;
    delete m_fountainNode;

	if (!useEvent->isPlaying()) useEvent->stop();
	useEvent->release();
	delete useEvent;

	if(cantUseEvent->isPlaying()) cantUseEvent->stop();
	cantUseEvent->release();
	delete cantUseEvent;
}

void Fountain::CreateFountain(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	float TMass = 0;

	// Create graphic body loading mesh
	m_fountainNode = g_engine->addObjMeshSceneNode("./../assets/modelos/fountain.obj");
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
			if (value < maxValue) { //Reload till reach 100
				Recover();
			}
							
			if (user != nullptr) { //Free the user once
				SetFree();
			}		
		}

		currentTime = 0.0f;
	}
	inUse = false;
}

void Fountain::Update(){
	std::cout<<"NO USAR ESTE UPDATE"<<std::endl;
}

void Fountain::SetFree(){
	if (useEvent->isPlaying()) useEvent->stop();
	user = nullptr;
	inUse = false;
}

bool Fountain::Use(){
	if(user!=nullptr){
		if(user->GetHP() < 100 || user->GetMP() < 100) { //Only use when HP or MP below 100
			if(incrementUse<=value){		
				value -= incrementUse;
				user->ChangeHP(incrementUse);
				user->ChangeMP(incrementUse);
				
				if (value == 0) {
					if (useEvent->isPlaying()) useEvent->stop();
				} else {
					playSoundEvent(useEvent);	//Play the sound
					useEvent->setParamValue("Fountain reserve", value); //Set event parameter for pitch modification
				}
				
				return true;
			} else {	//If the fountain has no reserve, stop the sound
				if (useEvent->isPlaying()) useEvent->stop();
				playSoundEvent(cantUseEvent);
			}	
		} else {
			SetFree(); 	//Free the fountain when the user has everything maxed
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
}

void Fountain::Interact(Player* p){
	if(user==nullptr){
		if (p->GetHP() < 100 || p->GetMP() < 100) { //Only interact when you something is below 100
			user = p;
			
		} else {
			playSoundEvent(cantUseEvent);
		}
	}
	inUse = true;
}

void Fountain::ShowInteractInfo(){
	GUIEngine::GetInstance()->ShowEntityInfo("[E] Drink");
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
	// Calculamos el punto delante de la fuente en vez del central
	vector3df posInit = bt_body->GetPosition();
	float dist = +0.6f;
	posInit.X += cos(rotation.Y)*dist;
	posInit.Z += sin(rotation.Y)*dist;

	cKin.position = posInit;
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

void Fountain::createSoundEvent() {
	useEvent = SoundSystem::getInstance()->createEvent("event:/CommonSounds/Fountains/Fountain");
	cantUseEvent = SoundSystem::getInstance()->createEvent("event:/HUD/Spell Disabled");
}

void Fountain::playSoundEvent(SoundEvent* event) {
	if (!event->isPlaying())
		SoundSystem::getInstance()->playEvent(event, bt_body->GetPosition());
}