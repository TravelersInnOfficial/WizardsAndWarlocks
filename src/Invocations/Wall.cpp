#include "Wall.h"

#include <ColliderMasks.h>
#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GBody.h>
#include <Assets.h>

InvocationWall::InvocationWall(int HP, float time, vector3df TPosition, vector3df TScale, vector3df TRotation)
:Invocation(HP){
	currentTime = 3;
	CreateInvocation(TPosition, TScale, TRotation);
}

bool InvocationWall::Update(float deltaTime){
	currentTime -= deltaTime;
	if(currentTime<=0){
		return false;
	}
	else if(m_HP == 0){
		return false;
	}
	return true;
}

void InvocationWall::CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	// Create a cube
	m_invocationNode = g_engine->addCube2Scene(TPosition, TRotation, TScale, 1.0f);
	m_invocationNode->setPosition(TPosition);
	m_invocationNode->setScale(TScale);

	if (m_invocationNode) m_invocationNode->setMaterialTexture(0, TEXTUREMAP[TEXTURE_SPELL_WALL]);

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,0,0, vector3df(0,0,0), C_WALL, wallCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}
