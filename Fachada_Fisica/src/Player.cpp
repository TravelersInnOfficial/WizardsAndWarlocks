#include "./Player.h"

Player::Player(){
	m_posX = 0;
	m_posY = 0;
	m_posZ = 0;
	m_width = 1;
	m_height = 1;
	m_length = 1;
	m_mass = 65;
	bt_player = new BT_Body();

} 

void Player::CreatePlayer(irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){
	//BULLET
	bt_player->CreateBox(m_posX,m_posY,m_posZ,m_width,m_height,m_length,m_mass);

    // IRRLICHT
    // Cargamos el Cubo
	m_playerNode = sceneManager->addCubeSceneNode();

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture( 0, driver->getTexture("./../assets/textures/wall.bmp"));
	}

	//sceneManager->addCameraSceneNodeFPS();
	sceneManager->addCameraSceneNode(0, irr::core::vector3df(0,0,0), irr::core::vector3df(0,0,0)); // X(+ = Dcha), Y (+ = Arriba), Z (+ = Delante)
	sceneManager->getActiveCamera()->setPosition(irr::core::vector3df(m_posZ, m_posY+20, m_posZ-80));
	sceneManager->getActiveCamera()->setTarget(irr::core::vector3df(m_posZ ,m_posY+20, m_posZ));
	sceneManager->getActiveCamera()->bindTargetAndRotation(true);

}

void Player::DeletePlayer(){
	bt_player->Erase();
}

void Player::Update(irr::scene::ISceneManager* sceneManager){
	//m_posX = GetPlayerTrans().getOrigin().getX();
	//m_posY = GetPlayerTrans().getOrigin().getY();
	//m_posZ = GetPlayerTrans().getOrigin().getZ();

	bt_player->Update();

	m_posX = bt_player->GetPosX();
	m_posY = bt_player->GetPosY();
	m_posZ = bt_player->GetPosZ();

	m_playerNode->setPosition( irr::core::vector3df(m_posX ,m_posY, m_posZ));

	//btVector3 velocity = m_playerRigidBody->getLinearVelocity();
    //btScalar speed = velocity.length();
    //if(speed > 20) {
    //    velocity *= 20/speed;
    //    m_playerRigidBody->setLinearVelocity(velocity);
    //}
}

void Player::Jump(){
	if(m_posY < 1){
		//m_playerRigidBody->applyCentralImpulse(btVector3(0,9.8*65,0));
		//m_posY = GetPlayerTrans().getOrigin().getY();

		bt_player->ApplyCentralImpulse(0,9.8*65,0);
		m_posY = bt_player->GetPosY();
	}
}

void Player::MoveX(int dir){
	//m_playerRigidBody->applyCentralImpulse(btVector3(dir*30,0,0));
	bt_player->ApplyCentralImpulse(dir*30,0,0);
	m_posX = bt_player->GetPosX();
}

void Player::MoveZ(int dir){
	//m_playerRigidBody->applyCentralImpulse(btVector3(0,0,dir*30));
	bt_player->ApplyCentralImpulse(0,0,dir*30);
	m_posZ = bt_player->GetPosZ();
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
