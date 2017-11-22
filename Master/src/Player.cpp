#include "./Player.h"

Player::Player(){
	m_position = new vector3df(0,0,0);
	m_dimensions = new vector3df(1,1,1);

	m_HP = 100;
	m_dead = false;
} 

void Player::CreatePlayer(irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){
	//IRRLICHT
	// Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode(m_dimensions->X);
	m_playerNode->setScale(irr::core::vector3df(m_dimensions->X, m_dimensions->Y, m_dimensions->Z));

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture(0, driver->getTexture("./../assets/textures/wall.bmp"));
		m_playerNode->setPosition(irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));
	}
	
	//BULLET
	m_dimensions->X = m_dimensions->X * 0.5f;
	m_dimensions->Y = m_dimensions->Y * 0.5f;
	m_dimensions->Z = m_dimensions->Z * 0.5f;
	
	bt_body = new BT_Body();
	bt_body->CreateBox(*m_position, *m_dimensions, 50, 2.3);

	Respawn(sceneManager);

}

void Player::DeletePlayer(){
	//BULLET
	bt_body->Erase();
}

void Player::Update(irr::scene::ISceneManager* sceneManager, bool isPlayerOne){
	if(m_position->Y < -50) Respawn(sceneManager);

	m_position->X = bt_body->GetPosition()->X;
	m_position->Y = bt_body->GetPosition()->Y;
	m_position->Z = bt_body->GetPosition()->Z;

	//BULLET
	bt_body->Update();
	m_playerNode->setPosition( irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));

	setMaxVelocity();
	if(isPlayerOne) positionCamera(sceneManager);
	vector3df* velocity = bt_body->GetLinearVelocity();

	if(!canJump){
		float verticalSpeed = velocity->Y;
		float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
		if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
		lastVerticalSpeed = verticalSpeed;
	}

	if(moving) moving = false;
	else bt_body->SetLinearVelocity(vector3df(velocity->X/1.5, velocity->Y, velocity->Z/1.5));
	if(m_dead) Respawn(sceneManager);
}

void Player::positionCamera(irr::scene::ISceneManager* sceneManager){
	irr::core::vector3df newRot = sceneManager->getActiveCamera()->getRotation();
	irr::core::vector3df newRotAux = sceneManager->getActiveCamera()->getRotation();
	irr::core::vector3df rot = newRot * irr::core::PI / 180.0;
	
	// Poner rotacion de personaje
	newRot.X = 0; newRot.Z = 0;
	m_playerNode->setRotation(newRot);

	// Poner posicion de camara
	sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_position->X - 0.15 * sin(rot.Y), m_position->Y + 0.5, m_position->Z - 5 - 0.15 * cos(rot.Y)));
	sceneManager->getActiveCamera()->updateAbsolutePosition();
	sceneManager->getActiveCamera()->setRotation(newRotAux);
}

void Player::setMaxVelocity(){
	vector3df* velocity = bt_body->GetLinearVelocity();
	vector3df auxVelocity(velocity->X,0,velocity->Z);
	float speed = auxVelocity.length();
	
	float velY = velocity->Y;
    if(speed > 3) {
        auxVelocity.X *= 3/speed;
		auxVelocity.Z *= 3/speed;
		auxVelocity.setY(velY);
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
	bt_body->ApplyCentralImpulse(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y));
	moving = true;
}

void Player::MoveZ(int dir, irr::scene::ISceneManager* sceneManager){
	float impulse = 30;
	impulse *= dir;
	irr::core::vector3df rot =  sceneManager->getActiveCamera()->getRotation();
	rot = rot * irr::core::PI / 180.0;
	bt_body->ApplyCentralImpulse(impulse * sin(rot.Y), 0, impulse * cos(rot.Y));
	moving = true;
}

void Player::Jump(){
	if(canJump) {
		vector3df* velocity = bt_body->GetLinearVelocity();
		velocity->setY(0);
		float impulse = 30 * 9.8;
		bt_body->ApplyCentralImpulse(0,impulse,0);
		m_position->Y = bt_body->GetPosition()->Y;
		canJump = false;
	}
}

void Player::ChangeHP(float HP){
	if(m_HP + HP > 100) m_HP = 100;
	else if(m_HP + HP < 0){ m_HP = 0; m_dead = true;}
	else m_HP += HP;
}

void Player::Respawn(irr::scene::ISceneManager* sceneManager){
	setPosition(0, 5, 0, sceneManager);
	m_HP = 100;
	m_dead = false;
}

void Player::setPosition(float posX, float posY, float posZ, irr::scene::ISceneManager* sceneManager){
	m_position->X = posX;
	m_position->Y = posY;
	m_position->Z = posZ;
	m_playerNode->setPosition(irr::core::vector3df(m_position->X, m_position->Y, m_position->Z));
	m_playerNode->updateAbsolutePosition();
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

void Player::SetHP(float HP){ m_HP = HP; }
void Player::SetDead(bool flag){ m_dead = flag; }
bool Player::GetDead(){ return m_dead; }

float Player::GetPosX(){ return m_position->X; }
float Player::GetPosY(){ return m_position->Y; }
float Player::GetWidth(){ return m_dimensions->X; }
float Player::GetHeight(){ return m_dimensions->Y; }
float Player::GetLength(){ return m_dimensions->Z; }
float Player::GetHP(){ return m_HP; }