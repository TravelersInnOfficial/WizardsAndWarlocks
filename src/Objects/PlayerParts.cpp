#include "PlayerParts.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GBody.h>
#include <GraphicEngine/GParticle.h>
#include <PhysicsEngine/BT_Body.h>
#include <ColliderMasks.h>

PlayerParts::PlayerParts(Alliance alliance, vector3df position, vector3df scale, vector3df rotation){
	m_rotation = rotation;
	m_ragdollTime = 5.0f;
	m_force = 200.0f;
	m_angularForce = 3.0f;

	if(alliance == ALLIANCE_WARLOCK) GenerateWarlockBodyParts(position);
	else GenerateWizardBodyParts(position);

	for(int i = 0; i < m_physicParts.size(); i++){
		if(m_physicParts.at(i) != nullptr) GenerateForce(m_physicParts.at(i));
	}

	for(int i = 0; i < m_physicParts.size(); i++){
		if(m_physicParts.at(i) != nullptr) GenerateForce(m_physicParts.at(i));
	}
}

PlayerParts::~PlayerParts(){
	for(int i = 0; i < m_physicParts.size(); i++){
		if(m_physicParts.at(i) != nullptr) delete m_physicParts.at(i);
	}

	for(int i = 0; i < m_graphicParts.size(); i++){
		if(m_graphicParts.at(i) != nullptr) delete m_graphicParts.at(i);
	}

	m_physicParts.clear();
	m_graphicParts.clear();

	if(GraphicEngine::getInstance()->GetParticleActive()){
		for(int i = 0; i < m_particles.size(); i++){
			if(m_particles.at(i) != nullptr) delete m_particles.at(i);
		}
		m_particles.clear();
	}
}

void PlayerParts::GenerateBodyPart(vector3df bodyPartPosition, vector3df phyisicalScale, std::string objPath){
	GBody* newGraphicBody = nullptr;
	BT_Body* newPhysicBody = nullptr;	
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	newGraphicBody = g_engine->addObjMeshSceneNode(objPath, bodyPartPosition, m_rotation, vector3df(1, 1, 1));

	newPhysicBody = new BT_Body();
	newPhysicBody->CreateBox(bodyPartPosition, phyisicalScale, 40.0f, 0.0f, vector3df(0,0,0), C_BODYPART, bodypartCW);
	newPhysicBody->Rotate(m_rotation);

	m_physicParts.push_back(newPhysicBody);
	m_graphicParts.push_back(newGraphicBody);

	if(GraphicEngine::getInstance()->GetParticleActive()){
		GParticle* particle = new GParticle(bodyPartPosition);
		particle->SetTexture("./../assets/textures/particles/SnareParticle.png");
		particle->SetType(BLOOD_PARTICLE);
		particle->SetQuantityPerSecond(100);
		m_particles.push_back(particle);
	}
}

void PlayerParts::GenerateForce(BT_Body* bodyPart){
	vector3df forceVec;
	
	// Randomize
	forceVec.X = rand(); forceVec.Y = rand(); forceVec.Z = rand();
	if(rand()%10 > 5) forceVec.X = forceVec.X * -1;
	if(rand()%10 > 5) forceVec.Z = forceVec.Z * -1;
	forceVec.normalize();

	// Apply forces
	bodyPart->SetAngularVelocity(forceVec * m_angularForce);
	bodyPart->ApplyCentralImpulse(forceVec * m_force);
}

bool PlayerParts::Update(float deltaTime){
	SyncParts();
	if(GraphicEngine::getInstance()->GetParticleActive()){
		for(int i = 0; i < m_particles.size(); i++){
			if(m_particles.at(i) != nullptr) m_particles.at(i)->Update();
		}
	}
	m_ragdollTime -= deltaTime;
	bool toRet = true;
	if(m_ragdollTime <= 0) toRet = false;
	return toRet;
}

void PlayerParts::GenerateWarlockBodyParts(vector3df bodyPos){
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.2, 0.2), "./../assets/modelos/PlayerParts/Warlock/head.obj");
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.5, 0.2), "./../assets/modelos/PlayerParts/Warlock/body.obj");
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.2, 0.2), "./../assets/modelos/PlayerParts/Warlock/rightArm.obj");
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.2, 0.2), "./../assets/modelos/PlayerParts/Warlock/leftArm.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Warlock/rightLeg.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Warlock/leftLeg.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Warlock/ornament1.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Warlock/ornament2.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.5, 0.1), "./../assets/modelos/PlayerParts/Warlock/staff.obj");
	for(int i = 0; i < m_graphicParts.size(); i++){
		if(m_graphicParts.at(i) != nullptr) GraphicEngine::getInstance()->setTextureToBody(m_graphicParts.at(i), 0, "./../assets/textures/Warlock.png");
	}
}

void PlayerParts::GenerateWizardBodyParts(vector3df bodyPos){
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.2, 0.2), "./../assets/modelos/PlayerParts/Wizards/head.obj");
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.4, 0.2), "./../assets/modelos/PlayerParts/Wizards/body.obj");
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.2, 0.4), "./../assets/modelos/PlayerParts/Wizards/rightArm.obj");
	GenerateBodyPart(bodyPos, vector3df(0.2, 0.2, 0.4), "./../assets/modelos/PlayerParts/Wizards/leftArm.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Wizards/rightLeg.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Wizards/leftLeg.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Wizards/ornament1.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.1, 0.1), "./../assets/modelos/PlayerParts/Wizards/ornament2.obj");
	GenerateBodyPart(bodyPos, vector3df(0.1, 0.5, 0.1), "./../assets/modelos/PlayerParts/Wizards/staff.obj");
	for(int i = 0; i < m_graphicParts.size(); i++){
		if(m_graphicParts.at(i) != nullptr) GraphicEngine::getInstance()->setTextureToBody(m_graphicParts.at(i), 0, "./../assets/textures/Wizard.png");
	}
}

void PlayerParts::SyncParts(){
	int pSize = m_physicParts.size();
	int gSize = m_graphicParts.size();
	for(int i = 0; i < pSize && i < gSize; i++){
		BT_Body* p_body = m_physicParts.at(i);
		GBody* g_body = m_graphicParts.at(i);
		if(p_body != nullptr && g_body != nullptr){
			p_body->Update();
    		g_body->setPosition(p_body->GetPosition());
			vector3df finalRotation = p_body->GetRotation();

			g_body->setRotation(vector3df(finalRotation.X, finalRotation.Y, 0)*180/M_PI);
			g_body->Rotate(vector3df(0,0, finalRotation.Z)*180/M_PI);
			
			if(GraphicEngine::getInstance()->GetParticleActive()){
				if(m_particles.at(i) != nullptr) m_particles.at(i)->SetPos(p_body->GetPosition());
			}
		}
	}
}