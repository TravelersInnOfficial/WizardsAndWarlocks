#include "./Player.h"
#include "./PhysicsEngine/BulletEngine.h"
#include "./Managers/ObjectManager.h"
#include "./Managers/TrapManager.h"
#include "./Managers/SpellManager.h"
#include "./Managers/EffectManager.h"

#include "./Objects/Potion.h"
#include "./Trap.h"

GraphicEngine* engine = GraphicEngine::getInstance();

Player::Player(bool isPlayer1){
	m_position = vector3df(0,0,0);
	m_dimensions = vector3df(1,1,1);

	raycastDistance = 2.0f;
	max_velocity = 3.0f;

	potion = NULL;

	m_HP = 100;
	m_MP = 100;
	m_dead = false;
	isPlayerOne=isPlayer1;
	clase = EENUM_PLAYER;

	CreatePlayer();
}

Player::~Player(){
	bt_body->Erase();
    m_playerNode->Erase();	

	delete m_playerNode;
	delete bt_body;
}

void Player::CreatePlayer(){
	//IRRLICHT
	GraphicEngine* engine = GraphicEngine::getInstance();
	// Cargamos el Cubo
	m_playerNode = engine->addCube2Scene(m_dimensions);
	m_playerNode->setScale(vector3df(m_dimensions.X, m_dimensions.Y, m_dimensions.Z));

	// Aplicamos Material unlit y Textura
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture(0, "./../assets/textures/wall.bmp");
		m_playerNode->setPosition(vector3df(m_position.X, m_position.Y, m_position.Z));
	}
	
	//BULLET
	m_dimensions = m_dimensions * 0.5f;
	
	bt_body = new BT_Body();
	bt_body->CreateBox(m_position, m_dimensions, 50, 2.3);
	bt_body->AssignPointer(this);
	Respawn();

}

void Player::Update(){
	UpdatePosShape();

	if(isPlayerOne){
		vector3df newRot = engine->getActiveCamera()->getRotation();
		vector3df rot = newRot * irr::core::PI / 180.0;	
		SetRotation(rot);
		positionCamera();
	}

	checkMaxVelocity();
	vector3df* velocity = bt_body->GetLinearVelocity();

	if(!canJump){
		float verticalSpeed = velocity->Y;
		float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
		if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
		lastVerticalSpeed = verticalSpeed;
	}

	if(moving) moving = false;
	else bt_body->SetLinearVelocity(vector3df(velocity->X/1.5, velocity->Y, velocity->Z/1.5));

	if(m_dead || m_position.Y < -50) Die();
}

void Player::positionCamera(){
	vector3df newRot = engine->getActiveCamera()->getRotation();
	vector3df newRotAux = engine->getActiveCamera()->getRotation();

	// Poner posicion de camara
	engine->getActiveCamera()->setPosition(vector3df(m_position.X /*- 0.15 * sin(rot.Y)*/, m_position.Y /*+ 0.5*/, m_position.Z /*- 0.15 * cos(rot.Y)*/));
	engine->getActiveCamera()->updateAbsolutePosition();
	engine->getActiveCamera()->setRotation(newRotAux);
}

void Player::checkMaxVelocity(){
	vector3df* velocity = bt_body->GetLinearVelocity();
	vector3df auxVelocity(velocity->X,0,velocity->Z);
	float speed = auxVelocity.length();
	
	float velY = velocity->Y;
    if(speed > max_velocity) {
        auxVelocity.X *= max_velocity/speed;
		auxVelocity.Z *= max_velocity/speed;
		auxVelocity.setY(velY);
		bt_body->SetLinearVelocity(auxVelocity);
    }
}

void Player::Move(float posX, float posY){
	SetPosX(posX);
	SetPosY(posY);
}

void Player::MoveX(int dir){
	float impulse = 30;
	impulse *= dir;
	vector3df rot = this->rotation;
	bt_body->ApplyCentralImpulse(vector3df(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y)));
	moving = true;
}

void Player::MoveZ(int dir){
	float impulse = 30;
	impulse *= dir;
	vector3df rot = this->rotation;
	bt_body->ApplyCentralImpulse(vector3df(impulse * sin(rot.Y), 0, impulse * cos(rot.Y)));
	moving = true;
}

void Player::Jump(){
	if(canJump) {
		vector3df* velocity = bt_body->GetLinearVelocity();
		velocity->setY(0);
		float impulse = 30 * 9.8;
		bt_body->ApplyCentralImpulse(vector3df(0,impulse,0));
		m_position.Y = bt_body->GetPosition().Y;
		canJump = false;
	}
}

void Player::ChangeHP(float HP){
	if(m_HP + HP > 100) m_HP = 100;
	else if(m_HP + HP <= 0){ m_HP = 0; m_dead = true;}
	else m_HP += HP;

	std::cout<<m_HP<<std::endl;
}

bool Player::ChangeMP(float MP){
	if(m_MP - MP < 0) return false;
	else{
		m_MP -= MP;
		return true;
	}
}

void Player::Respawn(){
	setPosition(0, 5, 0);
	m_HP = 100;
	m_dead = false;
}

void Player::Raycast(){
	vector3df rot = GetRot();
	rot.X = -rot.X;

	vector3df Start = GetPos();
	float EndX = Start.X + sin(rot.Y)*cos(rot.X)*raycastDistance;
	float EndY = Start.Y + sin(rot.X)*raycastDistance;
	float EndZ = Start.Z + cos(rot.Y)*cos(rot.X)*raycastDistance;

	vector3df End(EndX, EndY, EndZ);

	void* Object = BulletEngine::GetInstance()->Raycast(Start, End);
	if(Object!=NULL){
		Entidad* h = (Entidad*)Object;
		h->Interact(this);
	}
}

void Player::Die(){
	EffectManager::GetInstance()->CleanEffects(this);
	Respawn();
}

void Player::CatchObject(Potion* p){
	potion = p;
}

void Player::DropObject(){
	if(potion!=NULL){
		potion->CreatePotion(m_position, vector3df(0,0,0));		//rotacion de la pocion fija?
		potion = NULL;
	}
}

void Player::UseObject(){
	if(potion!=NULL){
		potion->Use(this);
		ObjectManager::GetInstance()->DeletePotion(potion);
		potion = NULL;
	}
}

void Player::DeployTrap(){

	vector3df rot = GetRot();
	rot.X = -rot.X;

	vector3df Start = GetPos();
	float EndX = Start.X + sin(rot.Y)*cos(rot.X)*raycastDistance;
	float EndY = Start.Y + sin(rot.X)*raycastDistance;
	float EndZ = Start.Z + cos(rot.Y)*cos(rot.X)*raycastDistance;

	vector3df End(EndX, EndY, EndZ);

	TrapManager::GetInstance()->AddTrap(End,TENUM_DEATH_CLAWS);
}

void Player::setPosition(float posX, float posY, float posZ){
	m_position.X = posX;
	m_position.Y = posY;
	m_position.Z = posZ;
	m_playerNode->setPosition(m_position);
	m_playerNode->updateAbsolutePosition();
	bt_body->SetPosition(m_position);
	//if(isPlayerOne) engine->getActiveCamera()->setRotation(vector3df(0,0,0));
}

void Player::SetPosition(vector3df pos){
	m_position = pos;
	m_playerNode->setPosition(pos);
	m_playerNode->updateAbsolutePosition();
	bt_body->SetPosition(pos);
	//if(isPlayerOne) engine->getActiveCamera()->setRotation(vector3df(0,0,0));
}

void Player::SetPosX(float posX){
	m_position.X = posX;
	m_playerNode->setPosition(vector3df(m_position.X, m_position.Y, m_position.Z));
}

void Player::SetPosY(float posY){
	m_position.Y = posY;
	m_playerNode->setPosition(vector3df(m_position.X, m_position.Y, m_position.Z));
}

void Player::SetRotation(vector3df rotation){
	this->rotation = rotation;
	vector3df newRot = this->rotation;
	newRot.X = 0; newRot.Z = 0;
	m_playerNode->setRotation(newRot);
}

void Player::SetHP(float HP){ m_HP = HP; }
void Player::SetDead(bool flag){ m_dead = flag; }
void Player::SetMaxVelocity(float max){ max_velocity = max; }
void Player::SetNetworkObject(NetworkObject* newNetworkObject){ networkObject = newNetworkObject; }

float Player::GetRotY(){
	return rotation.Y;
}

vector3df Player::GetRot(){
	return rotation;
}

bool Player::IsPlayerOne(){ return(isPlayerOne); }
bool Player::GetDead(){ return m_dead; }
float Player::GetPosX(){ return m_position.X; }
float Player::GetPosY(){ return m_position.Y; }
float Player::GetPosZ(){ return m_position.Z; }
vector3df Player::GetPos(){ return m_position; }
float Player::GetWidth(){ return m_dimensions.X; }
float Player::GetHeight(){ return m_dimensions.Y; }
float Player::GetLength(){ return m_dimensions.Z; }
float Player::GetHP(){ return m_HP; }
float Player::GetMP(){ return m_MP; }
float Player::GetMaxVelocity(){ return max_velocity; }
NetworkObject* Player::GetNetworkObject(){ return (networkObject); }

void Player::UpdatePosShape(){
	m_position = bt_body->GetPosition();
	bt_body->Update();
	m_playerNode->setPosition( vector3df(m_position.X, m_position.Y, m_position.Z));
}