#include "./Player.h"

Player::Player(){
	m_position = new vector3df(0,0,0);
	m_dimensions = new vector3df(1,1,1);

	m_HP = 100;
	m_dead = false;
} 

void Player::CreatePlayer(irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){

/*
	// CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	const btVector3 shapeInfo(m_dimensions->X * 1 * 0.5f, m_dimensions->Y * 1 * 0.5f, m_dimensions->Z * 1 * 0.5f);
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

*/
	//BULLET
	//TODO::
	m_dimensions->X = m_dimensions->X * 1 * 0.5f;
	m_dimensions->Y = m_dimensions->Y * 1 * 0.5f;
	m_dimensions->Z = m_dimensions->Z * 1 * 0.5f;
	
	bt_body = new BT_Body();
	bt_body->CreateBox(*m_position, *m_dimensions, 50, 2.3);

	//IRRLICHT
	// Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode(m_dimensions->X);
	m_playerNode->setScale(irr::core::vector3df(1,1,1));

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
		m_playerNode->setPosition(irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));
	}

}

void Player::DeletePlayer(){

	/*
	dynamicsWorld->removeRigidBody(m_playerRigidBody);
    delete m_playerRigidBody->getMotionState();
    delete m_playerRigidBody;
    delete m_playerShape;*/

	//BULLET
	bt_body->Erase();
}

void Player::Update(irr::scene::ISceneManager* sceneManager, bool isPlayerOne){
	if(m_position->Y < -50) Respawn(sceneManager);

	/*
	m_position->X = GetPlayerTrans().getOrigin().getX();
	m_position->Y = GetPlayerTrans().getOrigin().getY();
	m_position->Z = GetPlayerTrans().getOrigin().getZ();
	*/

	m_position->X = bt_body->GetPosition()->X;
	m_position->Y = bt_body->GetPosition()->Y;
	m_position->Z = bt_body->GetPosition()->Z;

	//BULLET
	bt_body->Update();

	m_playerNode->setPosition( irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));

	setMaxVelocity();
	if(isPlayerOne) positionCamera(sceneManager);
	
	//btVector3 velocity = m_playerRigidBody->getLinearVelocity();
	vector3df* velocity = bt_body->GetLinearVelocity();

	if(!canJump){
		float verticalSpeed = velocity->Y;
		float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
		if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
		lastVerticalSpeed = verticalSpeed;
	}

	if(moving) moving = false;
	//else m_playerRigidBody->setLinearVelocity(btVector3(velocity.getX()/1.5,velocity.getY(),velocity.getZ()/1.5));
	else bt_body->SetLinearVelocity(vector3df(velocity->X/1.5, velocity->Y, velocity->Z/1.5));
}

void Player::positionCamera(irr::scene::ISceneManager* sceneManager){
	irr::core::vector3df newRot = sceneManager->getActiveCamera()->getRotation();
	irr::core::vector3df newRotAux = sceneManager->getActiveCamera()->getRotation();
	irr::core::vector3df rot = newRot * irr::core::PI / 180.0;
	
	// Poner rotacion de personaje
	newRot.X = 0; newRot.Z = 0;
	m_playerNode->setRotation(newRot);

	// Poner posicion de camara
	sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_position->X - 0.15 * sin(rot.Y), m_position->Y + 0.5, m_position->Z - 0.15 * cos(rot.Y)));
	sceneManager->getActiveCamera()->updateAbsolutePosition();
	sceneManager->getActiveCamera()->setRotation(newRotAux);
}

void Player::setMaxVelocity(){
	/*
	btVector3 velocity = m_playerRigidBody->getLinearVelocity();
	btVector3 auxVelocity(velocity.getX(),0,velocity.getZ());*/
	vector3df* velocity = bt_body->GetLinearVelocity();
	vector3df auxVelocity(velocity->X,0,velocity->Z);

	//btScalar speed = auxVelocity.length();
	float speed = auxVelocity.length();
	
	float velY = velocity->Y;
    if(speed > 3) {
        auxVelocity.X *= 3/speed;
		auxVelocity.Z *= 3/speed;
		auxVelocity.setY(velY);
        //m_playerRigidBody->setLinearVelocity(auxVelocity);
		bt_body->SetLinearVelocity(auxVelocity);
    }
}

void Player::Move(float posX, float posY){
	SetPosX(posX);
	SetPosY(posY);
}


void Player::MoveX(int dir, irr::scene::ISceneManager* sceneManager){
	float impulse = 30;
	impulse *= dir;
	irr::core::vector3df rot =  sceneManager->getActiveCamera()->getRotation();
	rot = rot * irr::core::PI / 180.0;
	//m_playerRigidBody->applyCentralImpulse(btVector3(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y)));
	bt_body->ApplyCentralImpulse(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y));
	moving = true;
}

void Player::MoveZ(int dir, irr::scene::ISceneManager* sceneManager){
	float impulse = 30;
	impulse *= dir;
	irr::core::vector3df rot =  sceneManager->getActiveCamera()->getRotation();
	rot = rot * irr::core::PI / 180.0;
	//m_playerRigidBody->applyCentralImpulse(btVector3(impulse * sin(rot.Y), 0, impulse * cos(rot.Y)));
	bt_body->ApplyCentralImpulse(impulse * sin(rot.Y), 0, impulse * cos(rot.Y));
	moving = true;
}

void Player::Jump(){
	if(canJump) {
		//btVector3 velocity = m_playerRigidBody->getLinearVelocity();
		vector3df* velocity = bt_body->GetLinearVelocity();
		velocity->setY(0);
		//m_playerRigidBody->setLinearVelocity(velocity);
		//bt_body-> SetLinearVelocity(velocity);

		float impulse = 30 * 9.8;
		//m_playerRigidBody->applyCentralImpulse(btVector3(0,impulse,0));
		bt_body->ApplyCentralImpulse(0,impulse,0);
		//m_position->Y = GetPlayerTrans().getOrigin().getY();
		m_position->Y = bt_body->GetPosition()->Y;
		canJump = false;
	}
}

void Player::ChangeHP(float HP){
	if(m_HP + HP > 100) m_HP = 100;
	
	else if(m_HP + HP < 0){
		m_HP = 0;
		m_dead = true;
	}

	else m_HP += HP;
}

void Player::Respawn(irr::scene::ISceneManager* sceneManager){
	setPosition(0, 5, 0, sceneManager);
	m_dead = false;
}

/*
btTransform Player::GetPlayerTrans(){
	btTransform trans;
    m_playerRigidBody->getMotionState()->getWorldTransform(trans);
    return trans;
}*/

void Player::setPosition(float posX, float posY, float posZ, irr::scene::ISceneManager* sceneManager){
	m_position->X = posX;
	m_position->Y = posY;
	m_position->Z = posZ;
	m_playerNode->setPosition(irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));
	m_playerNode->updateAbsolutePosition();

/*
	btTransform transform;
    m_playerRigidBody->getMotionState()->getWorldTransform(transform);
    transform.setOrigin(btVector3(m_position->X, m_position->Y, m_position->Z));
    m_playerRigidBody->getMotionState()->setWorldTransform(transform);
    m_playerRigidBody->setCenterOfMassTransform(transform);
	m_playerRigidBody->setLinearVelocity(btVector3(0,0,0));
	*/

	bt_body->SetPosition(vector3df(m_position->X, m_position->Y, m_position->Z));

	sceneManager->getActiveCamera()->setRotation(irr::core::vector3df(0,0,0));
}

void Player::SetPosX(float posX){
	m_position->X = posX;
	m_playerNode->setPosition(irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));
}

void Player::SetPosY(float posY){
	m_position->Y = posY;
	m_playerNode->setPosition(irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));
}

void Player::SetHP(float HP){
	m_HP = HP;
}

void Player::SetDead(bool flag){
	m_dead = flag;
}

float Player::GetPosX(){ return m_position->X; }
float Player::GetPosY(){ return m_position->Y; }
float Player::GetWidth(){ return m_dimensions->X; }
float Player::GetHeight(){ return m_dimensions->Y; }
float Player::GetLength(){ return m_dimensions->Z; }
float Player::GetHP(){ return m_HP; }
bool Player::GetDead(){ return m_dead; }