#include "./Player.h"

Player::Player(){
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_width = 1;
	m_height = 1;
	m_length = 1;
} 

void Player::CreatePlayer(btDiscreteDynamicsWorld* dynamicsWorld, irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){

	// CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	const btVector3 shapeInfo(m_width * 1 * 0.5f, m_height * 1 * 0.5f, m_length * 1 * 0.5f);
	m_playerShape = new btBoxShape(shapeInfo);

	// CREATE INITIAL MOTION STATE OF THE PLAYER
	// Con el 1 en offset contrarrestamos el -1 del suelo
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
	//m_playerRigidBody->activate(true);
	m_playerRigidBody->setActivationState(DISABLE_DEACTIVATION);
	m_playerRigidBody->setAngularFactor(btVector3(0,0,0));
	//m_playerRigidBody->setDamping(btScalar(0.9), btScalar(0));
	//m_playerRigidBody->setGravity(btVector3(0,-9.8,0));

	// IRRLICHT
	// We set the camera with an unused keymap so it doesn't move idnependently
	SKeyMap keyMap[2];
	keyMap[0].Action = EKA_CROUCH;
  	keyMap[0].KeyCode = KEY_UP;
	sceneManager->addCameraSceneNodeFPS(0, 120, 200, -1, keyMap, 1, true);

	// Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode(m_width);
	m_playerNode->setScale(vector3df(1,1,1));

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
		m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
		//m_playerNode->setAutomaticCulling(EAC_FRUSTUM_SPHERE );
	}

}

void Player::DeletePlayer(btDiscreteDynamicsWorld* dynamicsWorld){
	dynamicsWorld->removeRigidBody(m_playerRigidBody);
    delete m_playerRigidBody->getMotionState();
    delete m_playerRigidBody;

    delete m_playerShape;
}

void Player::Update(irr::scene::ISceneManager* sceneManager, bool isPlayerOne){
	m_posX = GetPlayerTrans().getOrigin().getX();
	m_posY = GetPlayerTrans().getOrigin().getY();
	m_posZ = GetPlayerTrans().getOrigin().getZ();

	m_playerNode->setPosition( irr::core::vector3df(m_posX, m_posY, m_posZ));


	btVector3 velocity = m_playerRigidBody->getLinearVelocity();
	btVector3 auxVelocity(velocity.getX(),0,velocity.getZ());
	btScalar speed = auxVelocity.length();
	float velY = velocity.getY();
    if(speed > 3) {
        auxVelocity *= 3/speed;
		auxVelocity.setY(velY);
        m_playerRigidBody->setLinearVelocity(auxVelocity);
    }

	if(isPlayerOne){
		//sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_posX, m_posY+3, m_posZ-5));
		sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
		vector3df newRot = sceneManager->getActiveCamera()->getRotation();
		newRot.X = 0; newRot.Z = 0;
		m_playerNode->setRotation(newRot);
		//cout<<"Vel en X: "<<(int)velocity.getX()<<". Vel en Y: "<<(int)velocity.getY()<<". Vel en Z: "<<(int)velocity.getZ()<<endl;
	}

	if(moving) moving = false;
	else m_playerRigidBody->setLinearVelocity(btVector3(velocity.getX()/1.5,velocity.getY(),velocity.getZ()/1.5));

}

void Player::Move(float posX, float posY){
	SetPosX(posX);
	SetPosY(posY);
	m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
}

void Player::Jump(){
	btVector3 velocity = m_playerRigidBody->getLinearVelocity();
	if(fabs(velocity.getY()) < 0.05f) {
		float impulse = 30 * 9.8;
		m_playerRigidBody->applyCentralImpulse(btVector3(0,impulse,0));
		m_posY = GetPlayerTrans().getOrigin().getY();
	}
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
	m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
}

void Player::SetPosY(float posY){
	m_posY = posY;
	m_playerNode->setPosition(irr::core::vector3df(m_posX, m_posY, m_posZ));
}
