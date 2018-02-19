#include "PlayerParts.h"

PlayerParts::PlayerParts(Alliance alliance, vector3df position, vector3df scale, vector3df rotation){
	m_rotation = rotation;
	m_ragdollTime = 5.0f;

	if(alliance == ALLIANCE_WARLOCK) GenerateWarlockBodyParts(position);
	else GenerateWizardBodyParts(position);

	for(int i = 0; i < m_physicParts.size(); i++){
		if(m_physicParts.at(i) != NULL) GenerateForce(m_physicParts.at(i));
	}
}

PlayerParts::~PlayerParts(){
	for(int i = 0; i < m_physicParts.size(); i++){
		if(m_physicParts.at(i) != NULL) delete m_physicParts.at(i);
	}

	for(int i = 0; i < m_graphicParts.size(); i++){
		if(m_graphicParts.at(i) != NULL) delete m_graphicParts.at(i);
	}

	m_physicParts.clear();
	m_graphicParts.clear();
}

void PlayerParts::GenerateBodyPart(vector3df bodyPartPosition, vector3df phyisicalScale, std::string objPath){
	GBody* newGraphicBody = NULL;
	BT_Body* newPhysicBody = NULL;	
	GraphicEngine* engine = GraphicEngine::getInstance();

	newGraphicBody = engine->addObjMeshSceneNode(objPath, bodyPartPosition, m_rotation, vector3df(1, 1, 1));
	newGraphicBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	newPhysicBody = new BT_Body();
	newPhysicBody->CreateBox(bodyPartPosition, phyisicalScale, 0, 0, vector3df(0,0,0), C_BODYPART, bodypartCW);
	newPhysicBody->Rotate(m_rotation);
}

void PlayerParts::GenerateForce(BT_Body* bodyPart){
	vector3df forceVec;
	forceVec.X = rand(); forceVec.Y = rand(); forceVec.Z = rand();
	forceVec.normalize();
	forceVec * m_force;
	bodyPart->ApplyCentralImpulse(forceVec);
}

bool PlayerParts::Update(float deltaTime){
	SyncParts();
	m_ragdollTime -= deltaTime;
	bool toRet = true;
	if(m_ragdollTime <= 0) toRet = false;
	return toRet;
}

void PlayerParts::GenerateWarlockBodyParts(vector3df bodyPos){
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
}

void PlayerParts::GenerateWizardBodyParts(vector3df bodyPos){
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
	GenerateBodyPart(bodyPos, vector3df(1, 1, 1), "./../assets/modelos/potion.obj");
}

void PlayerParts::SyncParts(){
	int pSize = m_physicParts.size();
	int gSize = m_graphicParts.size();
	for(int i = 0; i < pSize && i < gSize; i++){
		BT_Body* p_body = m_physicParts.at(i);
		GBody* g_body = m_graphicParts.at(i);
		if(p_body != NULL && g_body != NULL){
			p_body->Update();
    		g_body->setPosition(p_body->GetPosition());
		}
	}
}