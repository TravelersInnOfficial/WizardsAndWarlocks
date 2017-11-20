#include "Bloque.h"

void Bloque::CreateBloque(btDiscreteDynamicsWorld* dynamicsWorld, irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){

	// CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	const btVector3 shapeInfo(10.0f,10.0f,10.0f);
	m_playerShape = new btBoxShape(shapeInfo);

	// CREATE INITIAL MOTION STATE OF THE PLAYER
	// Con el 1 en offset contrarrestamos el -1 del suelo
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));

    // CREATE PLAYER INFO
    playerMass = 1;
    btVector3 playerInertia(0, 0, 0);
    m_playerShape->calculateLocalInertia(playerMass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotionState, m_playerShape, playerInertia);

    // ADD PLAYER TO THE WORLD
    m_playerRigidBody = new btRigidBody(playerRigidBodyCI);
    dynamicsWorld->addRigidBody(m_playerRigidBody);

    // IRRLICHT
    // Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode(20.0f);

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture( 0, driver->getTexture("./assets/textures/pared.bmp") );
		// m_playerNode->setPosition(irr::core::vector3df(0,0,0));
	}
}

void Bloque::Update(){
	m_posX = GetPlayerTrans().getOrigin().getX();
	m_posY = GetPlayerTrans().getOrigin().getY();
	m_posZ = GetPlayerTrans().getOrigin().getZ();

	m_playerNode->setPosition( irr::core::vector3df(m_posX ,m_posY, m_posZ));
}

btTransform Bloque::GetPlayerTrans(){
	btTransform trans;
    m_playerRigidBody->getMotionState()->getWorldTransform(trans);
    return trans;
}