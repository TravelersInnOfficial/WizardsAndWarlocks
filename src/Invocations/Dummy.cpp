#include "Dummy.h"

Dummy::Dummy(int HP, float time, bool alli, vector3df TPosition, vector3df TScale, vector3df TRotation)
:Invocation(HP){
	currentTime = time;
	alliance = alli;
	CreateInvocation(TPosition, TScale, TRotation);
}

void Dummy::CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation){
	// Soundsystme
	playSpawnEvent(TPosition);

	// GraphicEngine
	GraphicEngine* engine = GraphicEngine::getInstance();

	if(alliance) {
		m_invocationNode = engine->addObjMeshSceneNode("./../assets/modelos/Wizard.obj");
		m_invocationNode->setMaterialTexture(0, "./../assets/textures/Wizard.png");
	}	
	else{
		m_invocationNode = engine->addObjMeshSceneNode("./../assets/modelos/Warlock.obj");
		m_invocationNode->setMaterialTexture(0, "./../assets/textures/Warlock.png");
	}

	m_invocationNode->setPosition(TPosition);
    m_invocationNode->setRotation(TRotation);
    m_invocationNode->setScale(vector3df(1,1,1));

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents, 50, 2.3, vector3df(0,0,0),C_PLAYER, playerCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

bool Dummy::Update(float deltaTime){
	currentTime -= deltaTime;
	if(currentTime<=0){
		return false;
	}
	else if(m_HP == 0){
		return false;
	}
	UpdatePosShape();
	return true;
}

void Dummy::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_invocationNode->setPosition(pos);
}