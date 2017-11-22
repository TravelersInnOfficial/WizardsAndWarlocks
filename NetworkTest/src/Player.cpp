#include "./Player.h"

//1m bullet = 50px sfml

Player::Player(){
	m_posX = 0.f;
	m_posY = 0.f;
	m_width = 1.f;
	m_height = 1.f;
	m_length = 1.f;

} 

void Player::CreatePlayer(btDiscreteDynamicsWorld* dynamicsWorld){

	//CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	const btVector3 shapeInfo(m_width,m_height,m_length);
	m_playerShape = new btBoxShape(shapeInfo);

	//CREATE INITIAL MOTION STATE OF THE PLAYER
    btDefaultMotionState* playerMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0))); //con el 1 en offset contrarrestamos el -1 del suelo

    //CREATE PLAYER INFO
    playerMass = 1;
    btVector3 playerInertia(0, 0, 0);
    m_playerShape->calculateLocalInertia(playerMass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotionState, m_playerShape, playerInertia);

    //ADD PLAYER TO THE WORLD
    m_playerRigidBody = new btRigidBody(playerRigidBodyCI);
    dynamicsWorld->addRigidBody(m_playerRigidBody);
}

void Player::DeletePlayer(btDiscreteDynamicsWorld* dynamicsWorld){

	dynamicsWorld->removeRigidBody(m_playerRigidBody);
    delete m_playerRigidBody->getMotionState();
    delete m_playerRigidBody;

    delete m_playerShape;
}

void Player::Update(){
	btTransform trans;
    m_playerRigidBody->getMotionState()->getWorldTransform(trans);
	m_posY = trans.getOrigin().getY();
}

void Player::Jump(){
	if(m_posY < 1)
		m_playerRigidBody->applyCentralImpulse(btVector3(0,50,0));
}

float Player::GetPosX(){
	return m_posX;
}

float Player::GetPosY(){
	return m_posY;
}

float Player::GetWidth(){
	return m_width;
}

float Player::GetHeight(){
	return m_height;
}

float Player::GetLength(){
	return m_length;
}

void Player::SetPosX(float posX){
	m_posX = posX;
}

void Player::SetPosY(float posY){
	m_posY = posY;
}
