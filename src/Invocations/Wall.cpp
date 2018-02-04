#include "Wall.h"

InvocationWall::InvocationWall(int HP, float time, vector3df TPosition, vector3df TScale, vector3df TRotation)
:Invocation(HP){
	currentTime = time;
	createSoundEvent();
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
	GraphicEngine* engine = GraphicEngine::getInstance();

	playSpawnEvent(TPosition);
	// Create an Irrlicht cube
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
	bt_body->CreateBox(TPosition, HalfExtents,0,0, vector3df(0,0,0), C_WALL, wallCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void InvocationWall::createSoundEvent(){
	std::cout << "debug1" << std::endl;
	spawnEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Desperation Wall");
}
