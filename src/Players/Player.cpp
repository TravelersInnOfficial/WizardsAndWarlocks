#include "./Player.h"
#include "./../PhysicsEngine/BulletEngine.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/EffectManager.h"

#include "./../Objects/Potion.h"
#include "./../Includes/TrapCodes.h"

GraphicEngine* engine = GraphicEngine::getInstance();

Player::Player(bool isPlayer1){
	m_position = vector3df(0,0,0);
	m_dimensions = vector3df(2,2,2);

	raycastDistance = 2.0f;
	max_velocity = 3.0f;

	playerAlliance = NO_ALLIANCE;
	isPlayerOne=isPlayer1;
	clase = EENUM_PLAYER;

	PlayerInit();
	CreatePlayer();
}

void Player::PlayerInit(){
	potion = NULL;
	m_HP = 100;
	m_MP = 100;
	m_dead = false;
}

Player::~Player(){
	bt_body->Erase();
    m_playerNode->Erase();
	delete m_playerNode;
	delete bt_body;
}

void Player::CreatePlayer(){

	// Graphic Player
	GraphicEngine* engine = GraphicEngine::getInstance();
	m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/npc.obj");
	m_playerNode->setScale(m_dimensions);
	if (m_playerNode) {
		m_playerNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
		m_playerNode->setMaterialTexture(0, "./../assets/textures/Wizard.png");
		m_playerNode->setPosition(m_position);
	}

	// Physic Player
	vector3df HalfExtents(m_dimensions.X * 0.15f, m_dimensions.Y * 0.35, m_dimensions.Z * 0.15f);
	bt_body = new BT_Body();
	bt_body->CreateBox(m_position, HalfExtents, 50, 2.3, vector3df(0,0,0),C_PLAYER, playerCW);
	bt_body->AssignPointer(this);

	TrapManager::GetInstance()->AddTrapToPlayer(this,TENUM_DEATH_CLAWS);
	
	Respawn();

}

void Player::GetNetInput(){

}

void Player::SetNetInput(){

}

void Player::Update(){
	UpdatePosShape();
	if(m_dead || m_position.Y < -50) Die();

	if(isPlayerOne){
		vector3df newRot = engine->getActiveCamera()->getRotation();
		vector3df rot = newRot * M_PI / 180.0;	
		SetRotation(rot);
		positionCamera();
	}

	checkMaxVelocity();
	vector3df velocity = bt_body->GetLinearVelocity();

	if(!canJump){
		float verticalSpeed = velocity.Y;
		float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
		if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
		lastVerticalSpeed = verticalSpeed;
	}

	if(moving) moving = false;
	else bt_body->SetLinearVelocity(vector3df(velocity.X/1.5, velocity.Y, velocity.Z/1.5));
}

void Player::positionCamera(){
	vector3df newRot = engine->getActiveCamera()->getRotation();
	engine->getActiveCamera()->setPosition(GetHeadPos());
	engine->getActiveCamera()->updateAbsolutePosition();
	engine->getActiveCamera()->setRotation(newRot);
}

void Player::checkMaxVelocity(){
	vector3df velocity = bt_body->GetLinearVelocity();
	vector3df auxVelocity(velocity.X,0,velocity.Z);
	float speed = auxVelocity.length();
	
	float velY = velocity.Y;
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
		vector3df velocity = bt_body->GetLinearVelocity();
		velocity.setY(0);
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
}

bool Player::ChangeMP(float MP){
	bool toRet = false;
	
	if(m_MP - MP >= 0){
		m_MP -= MP;
		toRet = true;
	}

	return (toRet);
}

void Player::Respawn(){
	SetPosition(vector3df(0, 1, 0));
	m_HP = 100;
	m_MP = 100;
	m_dead = false;
}

void Player::Raycast(){
	vector3df rot = GetRot();
	rot.X = -rot.X;

	vector3df Start = GetHeadPos();
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
	DropObject();
	EffectManager::GetInstance()->CleanEffects(this);
	Respawn();
}

void Player::CatchObject(Potion* p){
	DropObject();
	potion = p;
}

void Player::DropObject(){
	if(potion!=NULL){
		/*vector3df dropForce = m_position;
		float impulse = 20;
		vector3df cameraRot = GetRot();

		dropForce.X = sin(cameraRot.Y) * impulse;
		dropForce.Y = impulse/2;
		dropForce.Z = cos(cameraRot.Y) * impulse;*/

		potion->CreatePotion(m_position, vector3df(0,0,0));
		//potion->Drop(dropForce);
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

	vector3df Start = GetHeadPos();
	float trapRayDist = raycastDistance*1.5;
	float EndX = Start.X + sin(rot.Y)*cos(rot.X)*trapRayDist;
	float EndY = Start.Y + sin(rot.X)*trapRayDist;
	float EndZ = Start.Z + cos(rot.Y)*cos(rot.X)*trapRayDist;

	vector3df End(EndX, EndY, EndZ);

	void* Object = BulletEngine::GetInstance()->Raycast(Start, End);
	if(Object!=NULL){
		Entidad* h = (Entidad*)Object;
		if(h->GetClase() == EENUM_FLOOR) TrapManager::GetInstance()->PlayerDeployTrap(this,Start,End);
	}
}

void Player::SetPosition(vector3df pos){
	m_position = pos;
	m_playerNode->setPosition(pos);
	m_playerNode->updateAbsolutePosition();
	bt_body->SetPosition(pos);
}

void Player::SetPosX(float posX){
	m_position.X = posX;
	m_playerNode->setPosition(m_position);
}

void Player::SetPosY(float posY){
	m_position.Y = posY;
	m_playerNode->setPosition(m_position);
}

void Player::SetRotation(vector3df rotation){
	this->rotation = rotation;
	vector3df newRot = this->rotation;
	newRot.X = 0; newRot.Z = 0;
	newRot = newRot * 180 / M_PI;
	m_playerNode->setRotation(newRot);
	bt_body->SetRotation(newRot);
}

void Player::UpdatePosShape(){
	m_position = bt_body->GetPosition();
	bt_body->Update();
	m_playerNode->setPosition(m_position);
}

void Player::SetHP(float HP){m_HP = HP; }
void Player::SetDead(bool flag){ m_dead = flag; }
void Player::SetMaxVelocity(float max){ max_velocity = max; }
void Player::SetNetworkObject(NetworkObject* newNetworkObject){ networkObject = newNetworkObject; }
bool Player::IsPlayerOne(){ return(isPlayerOne); }


vector3df Player::GetAngularVelocity(){ return bt_body->GetAngularVelocity(); }
bool Player::GetDead(){ return m_dead; }
float Player::GetPosX(){ return m_position.X; }
float Player::GetPosY(){ return m_position.Y; }
float Player::GetPosZ(){ return m_position.Z; }
vector3df Player::GetPos(){ return m_position; }
float Player::GetRotY(){ return rotation.Y; }
vector3df Player::GetRot(){ return rotation; }
float Player::GetWidth(){ return m_dimensions.X; }
float Player::GetHeight(){ return m_dimensions.Y; }
float Player::GetLength(){ return m_dimensions.Z; }
float Player::GetHP(){ return m_HP; }
float Player::GetMP(){ return m_MP; }
float Player::GetMaxVelocity(){ return max_velocity; }
NetworkObject* Player::GetNetworkObject(){ return (networkObject); }
vector3df Player::GetVelocity(){return (bt_body->GetLinearVelocity());}

vector3df Player::GetHeadPos(){
	float offset = -0.1;
	vector3df headPos = m_position;
	vector3df cameraRot = GetRot();

	headPos.X += sin(cameraRot.Y) * offset;
	headPos.Y += 0.5; // Y OFFSET
	headPos.Z += cos(cameraRot.Y) * offset;

	return (headPos);
}

void Player::SetAlliance(Alliance newAlliance){
	playerAlliance = newAlliance;
	switch(newAlliance){
		case(ALLIANCE_WIZARD):{
			m_playerNode->setMaterialTexture(0, "./../assets/textures/Wizard.png");
			break;
		}
		case(ALLIANCE_WARLOCK):{
			m_playerNode->setMaterialTexture(0, "./../assets/textures/Warlock.png");
			break;
		}
		default:{
			m_playerNode->setMaterialTexture(0, "./../assets/textures/npc.png");
			break;
		}
	}
}