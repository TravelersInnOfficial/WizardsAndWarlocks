#include "Invocation.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

Invocation::Invocation(int HP, float time, vector3df pos, vector3df scale, vector3df rot){
	m_HP = HP;
	currentTime = time;
	clase = EENUM_INVOCATION;
	createSoundEvent();
	CreateInvocation(pos, scale, rot);

}

Invocation::~Invocation(){
	spawnEvent->release();
	bt_body->Erase();
    m_invocationNode->Erase();	

	delete m_invocationNode;
	delete bt_body;
}

void Invocation::CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* engine = GraphicEngine::getInstance();

	playSpawnEvent(TPosition);
	// Create an Irrlicht cube
	m_invocationNode = engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	m_invocationNode->setPosition(TPosition);
	m_invocationNode->setScale(TScale);
	m_invocationNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_invocationNode) {
		m_invocationNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_invocationNode->setMaterialTexture(0, "./../assets/textures/projectils/clouds.jpeg");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,0,0, vector3df(0,0,0), C_WALL, wallCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
	
}

void Invocation::Update(){
	//P KE NO WARNEE
}

bool Invocation::Update(float deltaTime){
	currentTime -= deltaTime;
	if(currentTime<=0){
		return false;
	}
	else if(m_HP == 0){
		return false;
	}
	return true;
}

void Invocation::Contact(void* punt, EntityEnum tipo){}

void Invocation::ChangeHP(int value){
	m_HP += value;
	if(m_HP<=0){
		m_HP = 0;
	}
}

void Invocation::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, true, AI_INVOCATION, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Invocation::GetKinematic(){
	Kinematic cKin;
	cKin.position = bt_body->GetPosition();
	cKin.orientation =  vector2df(0,0);
   	cKin.velocity = bt_body->GetLinearVelocity();
    cKin.rotation = vector2df(0,0);
    return cKin;
}

void Invocation::createSoundEvent()  {
	spawnEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/DesperationWallSpawn");
}

void Invocation::playSpawnEvent(vector3df pos) {
	SoundSystem::getInstance()->playEvent(spawnEvent, pos);
}