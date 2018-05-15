#include "Dummy.h"

#include "./../Managers/ObjectManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GAnimation.h>
#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GBody.h>
#include <ColliderMasks.h>
#include <Assets.h>

Dummy::Dummy(int HP, float time, bool alli, vector3df TPosition, vector3df TScale, vector3df TRotation)
:Invocation(HP){
	m_position = TPosition;
	currentTime = time;
	alliance = alli;
	CreateInvocation(TPosition, TScale, TRotation);
	
}

Dummy::~Dummy(){
	delete m_bottomBody;
}

void Dummy::CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation){

	//Bullet Physics
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, TScale, 50, 2.3, vector3df(0,0,0),C_PLAYER, playerCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);

	TPosition.Y += 0.3f;

	// GraphicEngine
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	m_invocationNode = g_engine->addAnimatedMeshSceneNode(TPosition, TRotation, vector3df(0.9, 0.9, 0.9));
	m_bottomBody = g_engine->addAnimatedMeshSceneNode(TPosition, TRotation, vector3df(0.9, 0.9, 0.9));

	GAnimation* topBody = (GAnimation*)m_invocationNode;

	if(alliance) {
		topBody->SetPaths("idle", AWIZARD_TIDLE);
		topBody->SetPaths("walk",  AWIZARD_TWALK);
		m_bottomBody->SetPaths("idle", AWIZARD_BIDLE);
		m_bottomBody->SetPaths("walk", AWIZARD_BWALK);

		topBody->setMaterialTexture(0, TEXTUREMAP[TEXTURE_Wizard]);
		m_bottomBody->setMaterialTexture(0, TEXTUREMAP[TEXTURE_Wizard]);
	}	
	else{
		topBody->SetPaths("idle", AWARLOCK_TIDLE);
		topBody->SetPaths("walk", AWARLOCK_TWALK);
		m_bottomBody->SetPaths("idle", AWARLOCK_BIDLE);
		m_bottomBody->SetPaths("walk", AWARLOCK_BWALK);

		topBody->setMaterialTexture(0, TEXTUREMAP[TEXTURE_Warlock]);
		m_bottomBody->setMaterialTexture(0, TEXTUREMAP[TEXTURE_Warlock]);
	}

	topBody->SetAnimationLoop("idle");
	m_bottomBody->SetAnimationLoop("idle");
}

void Dummy::SetName(std::string name){
	m_invocationNode->AddText(name, vector3df(0,1.25f,0), -1);
}

bool Dummy::Update(float deltaTime){
	currentTime -= deltaTime;
	if(currentTime<=0 || m_HP <= 0){
		ObjectManager::GetInstance()->AddPlayerParts((Alliance)alliance, bt_body->GetPosition(), vector3df(0.9, 0.9, 0.9), vector3df(0,0,0));
		return false;
	}
	CheckVelocity();
	UpdatePosShape(deltaTime);
	return true;
}

void Dummy::CheckVelocity(){
	vector3df vel = bt_body->GetLinearVelocity();
	vel.Y = 0;

	float length = vel.length();

	GAnimation* topBody = (GAnimation*)m_invocationNode;
	if(length >= 1.0f){
		topBody->SetAnimationLoop("walk");
		m_bottomBody->SetAnimationLoop("walk");
	}else{
		topBody->SetAnimationLoop("idle");
		m_bottomBody->SetAnimationLoop("idle");
	}

}

void Dummy::UpdatePosShape(float deltaTime){
	bt_body->Update();

    vector3df pos = bt_body->GetPosition();
    pos.Y += 0.3f;

    GAnimation* topBody = (GAnimation*)m_invocationNode;
    topBody->setPosition(pos);
    m_bottomBody->setPosition(pos);
    topBody->Update(deltaTime);
    m_bottomBody->Update(deltaTime);

}
