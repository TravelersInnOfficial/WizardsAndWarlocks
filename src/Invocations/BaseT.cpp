#include "BaseT.h"

BaseT::BaseT(vector3df TPosition, vector3df TScale, vector3df TRotation)
:Invocation(-1){
	active = true;
	CreateInvocation(TPosition, TScale, TRotation);
}

void BaseT::CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation){

	// GraphicEngine
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	m_invocationNode = g_engine->addObjMeshSceneNode("./../assets/modelos/restore_fountain.obj");
	m_invocationNode->setPosition(TPosition);
	m_invocationNode->setScale(TScale);
	m_invocationNode->setRotation(TRotation);

	if (m_invocationNode) m_invocationNode->setMaterialTexture(0, "./../assets/textures/teleport_base.png");

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents, 0, 2.3, vector3df(0,0,0),C_POTION, potionCW);
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

