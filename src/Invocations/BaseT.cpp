#include "BaseT.h"

BaseT::BaseT(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Invocation(-1){
	active = true;
	CreateInvocation(TPosition, TScale, TRotation);
}

void BaseT::CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation){
	// Soundsystme
	playSpawnEvent(TPosition);

	// GraphicEngine
	GraphicEngine* engine = GraphicEngine::getInstance();

	m_invocationNode = engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	m_invocationNode->setPosition(TPosition);
	m_invocationNode->setScale(TScale);
	m_invocationNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_invocationNode) {
		m_invocationNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_invocationNode->setMaterialTexture(0, "./../assets/textures/projectils/SPELL_WALL.png");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents, 50, 2.3, vector3df(0,0,0),C_POTION, potionCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

bool BaseT::Update(float deltaTime){
	return active;
}

void BaseT::Deactivate(){
	active = false;
}

vector3df BaseT::GetPosition(){
	return bt_body->GetPosition();
}