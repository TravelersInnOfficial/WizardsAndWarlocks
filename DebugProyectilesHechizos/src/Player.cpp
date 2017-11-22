#include "./Player.h"

Player::Player(){
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_width = 2;
	m_height = 2;
	m_length = 2;
	clase = "player";
} 

void Player::CreatePlayer(btDiscreteDynamicsWorld* dynamicsWorld, irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){

	// CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	const btVector3 HalfExtents(m_width/2,m_height/2,m_length/2);
	m_playerShape = new btBoxShape(HalfExtents);

	// CREATE INITIAL MOTION STATE OF THE PLAYER
	// Con el 1 en offset contrarrestamos el -1 del suelo
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));

    // CREATE PLAYER INFO
    playerMass = 80;
    btVector3 playerInertia(0, 0, 0);
    m_playerShape->calculateLocalInertia(playerMass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotionState, m_playerShape, playerInertia);

    // ADD PLAYER TO THE WORLD
    m_playerRigidBody = new btRigidBody(playerRigidBodyCI);
	dynamicsWorld->addRigidBody(m_playerRigidBody);
	m_playerRigidBody->activate(true);
	m_playerRigidBody->setActivationState(DISABLE_DEACTIVATION);

	//Le metemos dentro un puntero a el mismo
	m_playerRigidBody->setUserPointer(this);

    // IRRLICHT
    // Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode(m_width);

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture( 0, driver->getTexture("./assets/textures/wall.bmp"));
	}

	//sceneManager->addCameraSceneNodeFPS();
	sceneManager->addCameraSceneNode(0, irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0)); // X(+ = Dcha), Y (+ = Arriba), Z (+ = Delante)
	sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_posZ, m_posY+20, m_posZ-80));
	sceneManager->getActiveCamera()->setTarget(irr::core::vector3df(m_posZ ,m_posY+20, m_posZ));
	sceneManager->getActiveCamera()->bindTargetAndRotation(true);

}

void Player::DeletePlayer(btDiscreteDynamicsWorld* dynamicsWorld){
	dynamicsWorld->removeRigidBody(m_playerRigidBody);
    delete m_playerRigidBody->getMotionState();
    delete m_playerRigidBody;

    delete m_playerShape;
}

void Player::Update(){
	m_posX = GetPlayerTrans().getOrigin().getX();
	m_posY = GetPlayerTrans().getOrigin().getY();
	m_posZ = GetPlayerTrans().getOrigin().getZ();

	m_playerNode->setPosition( irr::core::vector3df(m_posX ,m_posY, m_posZ));


	btVector3 velocity = m_playerRigidBody->getLinearVelocity();
    btScalar speed = velocity.length();
    if(speed > 20) {
        velocity *= 20/speed;
        m_playerRigidBody->setLinearVelocity(velocity);
    }
}

void Player::Contact(void* punt, string tipo){
	
}

void Player::Move(float posX, float posY){
	SetPosX(posX);
	SetPosY(posY);
}

void Player::Jump(){
	if(m_posY < 1){
		m_playerRigidBody->applyCentralImpulse(btVector3(0,9.8*65,0));
		m_posY = GetPlayerTrans().getOrigin().getY();
	}
}

void Player::MoveX(int dir){
	m_playerRigidBody->applyCentralImpulse(btVector3(dir*30,0,0));
}

void Player::MoveZ(int dir){
	m_playerRigidBody->applyCentralImpulse(btVector3(0,0,dir*30));
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

btTransform Player::GetPlayerTrans(){
	btTransform trans;
    m_playerRigidBody->getMotionState()->getWorldTransform(trans);
    return trans;
}

void Player::SetPosX(float posX){
	m_posX = posX;
	// Ponerselo al objeto FISICO
}

void Player::SetPosY(float posY){
	m_posY = posY;
	// Ponerselo al objeto FISICO
}
