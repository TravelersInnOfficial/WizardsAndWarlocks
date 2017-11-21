#include "./Player.h"

Player::Player(){
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_width = 1;
	m_height = 1;
	m_length = 1;
	m_HP = 100;
	m_dead = false;
} 

void Player::CreatePlayer(btDiscreteDynamicsWorld* dynamicsWorld, irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){

	// CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	const btVector3 shapeInfo(m_width * 1 * 0.5f, m_height * 1 * 0.5f, m_length * 1 * 0.5f);
	m_playerShape = new btBoxShape(shapeInfo);

	// CREATE INITIAL MOTION STATE OF THE PLAYER
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));

    // CREATE PLAYER INFO
    playerMass = 50;
    btVector3 playerInertia(0, 0, 0);
    m_playerShape->calculateLocalInertia(playerMass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotionState, m_playerShape, playerInertia);
	playerRigidBodyCI.m_friction = 2.3;

    // ADD PLAYER TO THE WORLD
    m_playerRigidBody = new btRigidBody(playerRigidBodyCI);
	dynamicsWorld->addRigidBody(m_playerRigidBody);
	m_playerRigidBody->setActivationState(DISABLE_DEACTIVATION);
	m_playerRigidBody->setAngularFactor(btVector3(0,0,0));

	// Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode(m_width);
	m_playerNode->setScale(vector3df(1,1,1));

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
		m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
	}
	Respawn(sceneManager);

}

void Player::DeletePlayer(btDiscreteDynamicsWorld* dynamicsWorld){
	dynamicsWorld->removeRigidBody(m_playerRigidBody);
    delete m_playerRigidBody->getMotionState();
    delete m_playerRigidBody;
    delete m_playerShape;
}

void Player::Update(irr::scene::ISceneManager* sceneManager, bool isPlayerOne){
	if(m_posY < -50) Respawn(sceneManager);

	m_posX = GetPlayerTrans().getOrigin().getX();
	m_posY = GetPlayerTrans().getOrigin().getY();
	m_posZ = GetPlayerTrans().getOrigin().getZ();

	m_playerNode->setPosition( irr::core::vector3df(m_posX, m_posY, m_posZ));

	setMaxVelocity();
	if(isPlayerOne) positionCamera(sceneManager);
	
	btVector3 velocity = m_playerRigidBody->getLinearVelocity();

	if(!canJump){
		float verticalSpeed = velocity.getY();
		float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
		if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
		lastVerticalSpeed = verticalSpeed;
	}

	if(moving) moving = false;
	else m_playerRigidBody->setLinearVelocity(btVector3(velocity.getX()/1.5,velocity.getY(),velocity.getZ()/1.5));

	if(m_dead) Respawn(sceneManager);
}

void Player::positionCamera(irr::scene::ISceneManager* sceneManager){
	vector3df newRot = sceneManager->getActiveCamera()->getRotation();
	vector3df newRotAux = sceneManager->getActiveCamera()->getRotation();
	vector3df rot = newRot * PI / 180.0;
	
	// Poner rotacion de personaje
	newRot.X = 0; newRot.Z = 0;
	m_playerNode->setRotation(newRot);

	// Poner posicion de camara
	sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_posX - 0.15 * sin(rot.Y), m_posY + 0.5, m_posZ - 0.15 * cos(rot.Y)));
	sceneManager->getActiveCamera()->updateAbsolutePosition();
	sceneManager->getActiveCamera()->setRotation(newRotAux);
}

void Player::setMaxVelocity(){
	btVector3 velocity = m_playerRigidBody->getLinearVelocity();
	btVector3 auxVelocity(velocity.getX(),0,velocity.getZ());
	btScalar speed = auxVelocity.length();
	float velY = velocity.getY();
    if(speed > 3) {
        auxVelocity *= 3/speed;
		auxVelocity.setY(velY);
        m_playerRigidBody->setLinearVelocity(auxVelocity);
    }
}

void Player::Move(float posX, float posY){
	SetPosX(posX);
	SetPosY(posY);
}


void Player::MoveX(int dir, irr::scene::ISceneManager* sceneManager){
	float impulse = 30;
	impulse *= dir;
	vector3df rot =  sceneManager->getActiveCamera()->getRotation();
	rot = rot * PI / 180.0;
	m_playerRigidBody->applyCentralImpulse(btVector3(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y)));
	moving = true;
}

void Player::MoveZ(int dir, irr::scene::ISceneManager* sceneManager){
	float impulse = 30;
	impulse *= dir;
	vector3df rot =  sceneManager->getActiveCamera()->getRotation();
	rot = rot * PI / 180.0;
	m_playerRigidBody->applyCentralImpulse(btVector3(impulse * sin(rot.Y), 0, impulse * cos(rot.Y)));
	moving = true;
}

void Player::Jump(){
	if(canJump) {
		btVector3 velocity = m_playerRigidBody->getLinearVelocity();
		velocity.setY(0);
		m_playerRigidBody->setLinearVelocity(velocity);
		float impulse = 30 * 9.8;
		m_playerRigidBody->applyCentralImpulse(btVector3(0,impulse,0));
		m_posY = GetPlayerTrans().getOrigin().getY();
		canJump = false;
	}
}

void Player::ChangeHP(float HP){
	if(m_HP + HP > 100) m_HP = 100;
	else if(m_HP + HP < 0){ m_HP = 0; m_dead = true; }
	else m_HP += HP;
}

void Player::Respawn(irr::scene::ISceneManager* sceneManager){
	setPosition(0, 5, 0, sceneManager);
	m_HP = 100;
	m_dead = false;
}


btTransform Player::GetPlayerTrans(){
	btTransform trans;
    m_playerRigidBody->getMotionState()->getWorldTransform(trans);
    return trans;
}

void Player::setPosition(float posX, float posY, float posZ, irr::scene::ISceneManager* sceneManager){
	m_posX = posX;
	m_posY = posY;
	m_posZ = posZ;
	m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
	m_playerNode->updateAbsolutePosition();

	btTransform transform;
    m_playerRigidBody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(btVector3(m_posX, m_posY, m_posZ));
    m_playerRigidBody->getMotionState()->setWorldTransform(transform);
    m_playerRigidBody->setCenterOfMassTransform(transform);
	m_playerRigidBody->setLinearVelocity(btVector3(0,0,0));

	sceneManager->getActiveCamera()->setRotation(vector3df(0,0,0));
}

void Player::SetPosX(float posX){
	m_posX = posX;
	m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
}

void Player::SetPosY(float posY){
	m_posY = posY;
	m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
}

void Player::SetHP(float HP){
	m_HP = HP;
}

void Player::SetDead(bool flag){
	m_dead = flag;
}

float Player::GetPosX(){ return m_posX; }
float Player::GetPosY(){ return m_posY; }
float Player::GetWidth(){ return m_width; }
float Player::GetHeight(){ return m_height; }
float Player::GetLength(){ return m_length; }
float Player::GetHP(){ return m_HP; }
bool Player::GetDead(){ return m_dead; }