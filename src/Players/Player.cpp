#include "./Player.h"
#include "./../PhysicsEngine/BulletEngine.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

#include <TrapCodes.h>
#include "./../Objects/Potion.h"

GraphicEngine* engine = GraphicEngine::getInstance();

Player::Player(bool isPlayer1){
	m_position = vector3df(0,0,0);
	m_dimensions = vector3df(1.8,1.8,1.8);

	controller = new PlayerController();
	DeclareInput();

	raycastDistance = 2.0f;
	max_velocity = 3.0f;

	playerAlliance = NO_ALLIANCE;
	isPlayerOne = isPlayer1;
	clase = EENUM_PLAYER;

	bt_body = NULL;
	m_playerNode = NULL;
	networkObject = NULL;
	matchStarted = false;
	hasCharacter = false;
	readyToStart = false;
	bloodOverlayTime = 0;

	currentSpell = 0;
	numberSpells = 3;   // Rango de hechizos [0 a numberSpells]

	PlayerInit();

	CreatePlayerCharacter(true);
	Respawn();
}

void Player::PlayerInit(){
	potion = NULL;
	m_HP = 100;
	m_MP = 100;
	m_DamageMult = 1;	
	m_dead = false;
	EffectManager::GetInstance()->CleanEffects(this);
}

Player::~Player(){
	if(bt_body != NULL){
		bt_body->Erase();
		delete bt_body;
		bt_body = NULL;
	}

    if(m_playerNode != NULL){
		m_playerNode->Erase();
		delete m_playerNode;
		m_playerNode = NULL;
	}	
}

void Player::CreatePlayerCharacter(bool firstInit){
	if(!hasCharacter){

		// Graphic Player
		GraphicEngine* engine = GraphicEngine::getInstance();
		
		if(playerAlliance == ALLIANCE_WIZARD) {
			if(isPlayerOne) m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/WizardArm.obj");
			else m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/Wizard.obj");
			m_playerNode->setMaterialTexture(0, "./../assets/textures/Wizard.png");
		}
		
		else if(playerAlliance == ALLIANCE_WARLOCK){
			if(isPlayerOne) m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/WarlockArm.obj");
			else m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/Warlock.obj");
			m_playerNode->setMaterialTexture(0, "./../assets/textures/Warlock.png");
		}
		
		else{
			if(isPlayerOne) m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/WizardArm.obj");
			else m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/npc.obj");
			m_playerNode->setMaterialTexture(0, "./../assets/textures/npc.png");
		}
		
		if(firstInit) m_playerNode->setScale(m_dimensions);
		m_playerNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
		m_playerNode->setPosition(m_position);

		// Physic Player
		vector3df HalfExtents(m_dimensions.X * 0.15f, m_dimensions.Y * 0.45, m_dimensions.Z * 0.15f);
		bt_body = new BT_Body();
		bt_body->CreateBox(m_position, HalfExtents, 50, 2.3, vector3df(0,0,0),C_PLAYER, playerCW);
		bt_body->AssignPointer(this);

		TrapManager::GetInstance()->AddTrapToPlayer(this,TENUM_DEATH_CLAWS);

		if(isPlayerOne) engine->addCameraSceneNodeFPS(120.f, 0);
		hasCharacter = true;
	}
}

void Player::DestroyPlayerCharacter(){
	if(bt_body != NULL){
		bt_body->Erase();
		delete bt_body;
		bt_body = NULL;
	}

    if(m_playerNode != NULL){
		m_playerNode->Erase();
		delete m_playerNode;
		m_playerNode = NULL;
	}

	if(isPlayerOne) engine->addCameraSceneNodeFPS(120.f, 0.005);
	hasCharacter = false;
}

void Player::DeclareInput(){
	controller->AddAction(KEY_KEY_W, ACTION_MOVE_UP);
	controller->AddAction(KEY_KEY_S, ACTION_MOVE_DOWN);
	controller->AddAction(KEY_KEY_A, ACTION_MOVE_LEFT);
	controller->AddAction(KEY_KEY_D, ACTION_MOVE_RIGHT);
	controller->AddAction(KEY_KEY_E, ACTION_RAYCAST);
	controller->AddAction(KEY_SPACE, ACTION_JUMP);
	controller->AddAction(KEY_KEY_Z, ACTION_USE_OBJECT);
	controller->AddAction(KEY_KEY_X, ACTION_DROP_OBJECT);
	controller->AddAction(KEY_LBUTTON, ACTION_SHOOT);
	controller->AddAction(KEY_KEY_F, ACTION_DEPLOY_TRAP);
	controller->AddAction(KEY_WHEEL_UP, ACTION_CHANGE_SPELL_UP);
	controller->AddAction(KEY_WHEEL_DOWN, ACTION_CHANGE_SPELL_DOWN);
}

void Player::SetAllInput(keyStatesENUM state){
	controller->SetAllStatus(state);
	if(isPlayerOne && networkObject != NULL) {
		networkObject->SetIntVar(PLAYER_SET_ALL_INPUT, (keyStatesENUM)state, true, false);
	}
}

void Player::UpdateInput(){
	controller->UpdateOwnStatus();
	if(isPlayerOne) controller->Update();
}

void Player::CheckInput(){}

void Player::GetNetInput(){
	int alliance = networkObject->GetIntVar(PLAYER_ALLIANCE);
	if(alliance != (int)NO_ALLIANCE){
		SetAlliance((Alliance)alliance);
		alliance = (int)NO_ALLIANCE;
		networkObject->SetIntVar(PLAYER_ALLIANCE, alliance, false, false);
	}

	bool doCreateChar = networkObject->GetBoolVar(PLAYER_CREATE_CHAR);
	if(doCreateChar){
		CreatePlayerCharacter();
		doCreateChar = false;
		networkObject->SetBoolVar(PLAYER_CREATE_CHAR, doCreateChar, false, false);
	}

	bool doRespawn = networkObject->GetBoolVar(PLAYER_RESPAWN);
	if(doRespawn){
		Respawn();
		doRespawn = false;
		networkObject->SetBoolVar(PLAYER_RESPAWN, doRespawn, false, false);
	}

	bool isReady = networkObject->GetBoolVar(PLAYER_READY);
	readyToStart = isReady;

}

void Player::SetNetInput(){
}

void Player::Update(){

	// En el caso de que se cumpla alguna de las condiciones de muerte lo matamos
	if(m_dead || m_position.Y < -50) Die();

	// Si tenemos cuerpo fisico
	if(hasCharacter){
		vector3df velocity = bt_body->GetLinearVelocity();
		if(!canJump){
			float verticalSpeed = velocity.Y;
			float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
			if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
			lastVerticalSpeed = verticalSpeed;
		}

		// En el caso de que se estuviera moviendo en el frame anterior cambiamos la variable, mientras
		// que si no se estaba moviendo lo frenamos 
		if(moving) moving = false; 
		else bt_body->SetLinearVelocity(vector3df(velocity.X/1.5, velocity.Y, velocity.Z/1.5));

		// Comprobamos los Input del personaje
		CheckInput();

		// Actualizamos el cuerpo visual del personaje respecto al fisico
		UpdatePosShape();

		// En el caso de que sea el jugador 1 actualizamos su camara
		if(isPlayerOne){
			vector3df newRot = engine->getActiveCamera()->getRotation();
			vector3df rot = newRot * M_PI / 180.0;	
			SetRotation(rot);
			positionCamera();
		}

		// Comprobamos la velocidad maxima del jugador para que no se sobrepase
		checkMaxVelocity();
	}
}

void Player::ChangeCurrentSpell(int value){
	int tempCurrentSpell = currentSpell + value;
	if(tempCurrentSpell >=0 && tempCurrentSpell<= numberSpells){
		currentSpell = tempCurrentSpell;
	}
	// Notify
	if(networkObject != NULL) networkObject->SetIntVar(PLAYER_SPELL, currentSpell, true, false);
}

void Player::SetSpell(int value){
	currentSpell = value;
	if(currentSpell < 0) currentSpell = 0;
	else if (currentSpell > numberSpells) currentSpell = numberSpells;
}

void Player::positionCamera(){
	vector3df newRot = engine->getActiveCamera()->getRotation();
	engine->getActiveCamera()->setPosition(GetHeadPos());
	engine->getActiveCamera()->updateAbsolutePosition();
	engine->getActiveCamera()->setRotation(newRot);
}

void Player::checkMaxVelocity(){
	if(hasCharacter){
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
}

void Player::Move(float posX, float posY){
	SetPosX(posX);
	SetPosY(posY);
}

void Player::MoveX(int dir){
	if(hasCharacter){
		float impulse = 30;
		impulse *= dir;
		vector3df rot = this->rotation;
		bt_body->ApplyCentralImpulse(vector3df(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y)));
		moving = true;
	}
}

void Player::MoveZ(int dir){
	if(hasCharacter){
		float impulse = 30;
		impulse *= dir;
		vector3df rot = this->rotation;
		bt_body->ApplyCentralImpulse(vector3df(impulse * sin(rot.Y), 0, impulse * cos(rot.Y)));
		moving = true;
	}
}

void Player::Jump(){
	if(canJump && hasCharacter) {
		vector3df velocity = bt_body->GetLinearVelocity();
		velocity.setY(0);
		float impulse = 30 * 9.8;
		bt_body->ApplyCentralImpulse(vector3df(0,impulse,0));
		m_position.Y = bt_body->GetPosition().Y;
		canJump = false;
	}
}

void Player::ChangeHP(float HP){

	if (HP < 0) {
		 SoundSystem::getInstance()->playEvent("event:/Character/Hard/Hit", GetPos(), GetRot()); //PLay the sound event
		 bloodOverlayTime = 1;
		 } 

	if(m_HP + HP > 100) m_HP = 100;
	
	else if(m_HP + HP <= 0){
		m_HP = 0;
		m_dead = true;
		bloodOverlayTime = 0;
	}

	else m_HP += HP;
}

bool Player::ChangeMP(float MP){
	bool toRet = false;

	if(m_MP + MP >= 0){
		m_MP += MP;
		toRet = true;

		if(m_MP>100) m_MP = 100;
	}

	return (toRet);
}

void Player::Respawn(){
	SetPosition(ObjectManager::GetInstance()->GetRandomSpawnPoint(playerAlliance));
	PlayerInit();
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

bool Player::StartSpell(){
	EffectManager* effectman = EffectManager::GetInstance();
	if(effectman->CheckEffect(this, WEAK_SILENCED) && currentSpell!=0){		// if is not a basic spell or if silenced then not shoot
		return false;
	}
	return SpellManager::GetInstance()->StartHechizo(currentSpell,this);
}

bool Player::ShootSpell(){
	return SpellManager::GetInstance()->LanzarHechizo(currentSpell,this);
}

void Player::ResetSpell(){
	SpellManager::GetInstance()->ResetHechizo(currentSpell,this);
}

void Player::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, false, AI_PLAYER, 5.0f, GetKinematic(), AI_SIGHT);
	if(moving){
		sense->AddSignal(id, false, AI_PLAYER, 5.0f, GetKinematic(), AI_HEARING);
	}
}

void Player::Die(){

	SoundSystem::getInstance()->checkAndPlayEvent("event:/Character/Hard/Die", GetPos(), GetRot()); //Play the sound event

	DropObject();

	if(matchStarted){
		PlayerManager::GetInstance()->AddToDead(playerAlliance, this);
		DestroyPlayerCharacter();
	}

	Respawn();
}

void Player::ReturnToLobby(){
	CreatePlayerCharacter();
	Respawn();
	if(networkObject != NULL){
		networkObject->SetBoolVar(PLAYER_CREATE_CHAR, true, true, false);
		networkObject->SetBoolVar(PLAYER_RESPAWN, true, true, false);
	}
}

void Player::DrawOverlays(float deltaTime){
	bloodOverlayTime -= deltaTime;
	if(bloodOverlayTime > 0) engine->drawOverlays(0);
}

void Player::CheckIfReady(){
	vector4df readyZone = ObjectManager::GetInstance()->GetReadyZone();
	
	bool ready = true;
	if(m_position.X < readyZone.X || m_position.X > readyZone.X2 || m_position.Z < readyZone.Y || m_position.Z > readyZone.Y2){
		ready = false;
	}
	
	readyToStart = ready;
	if(networkObject != NULL) networkObject->SetBoolVar(PLAYER_READY, ready, true, false);
}

void Player::CatchObject(Potion* p){
	DropObject();
	potion = p;
}

void Player::DropObject(){
	if(potion!=NULL){
		potion->CreatePotion(m_position, vector3df(0,0,0));

		/*vector3df dropForce = m_position;
		float impulse = 20;
		vector3df cameraRot = GetRot();

		dropForce.X = sin(cameraRot.Y) * impulse;
		dropForce.Y = impulse/2;
		dropForce.Z = cos(cameraRot.Y) * impulse;
		potion->Drop(dropForce);*/

		potion = NULL;
	}
}

void Player::UseObject(){
	if(potion!=NULL){
		SoundSystem::getInstance()->checkAndPlayEvent("event:/Character/Hard/Drink", GetPos(), GetRot());
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

void Player::UpdatePosShape(){
	if(hasCharacter){
		m_position = bt_body->GetPosition();
		bt_body->Update();
		m_playerNode->setPosition(m_position);

		rotation = bt_body->GetRotation();
		m_playerNode->setRotation(rotation * 180 / M_PI);
	}
}

bool Player::IsPlayerOne(){ return(isPlayerOne); }

vector3df Player::GetAngularVelocity(){
	vector3df toRet = vector3df(-999,-999,-999);
	if(hasCharacter) toRet = bt_body->GetAngularVelocity();
	return toRet;
}

bool Player::GetDead(){ return m_dead; }

vector3df Player::GetPos(){ return m_position; }

float Player::GetRotY(){ return rotation.Y; }

vector3df Player::GetRot(){ return rotation; }

float Player::GetWidth(){ return m_dimensions.X; }

float Player::GetHeight(){ return m_dimensions.Y; }

float Player::GetLength(){ return m_dimensions.Z; }

float Player::GetHP(){ return m_HP; }

float Player::GetMP(){ return m_MP; }

float Player::GetDamageM(){ return m_DamageMult; }

float Player::GetMaxVelocity(){ return max_velocity; }

NetworkObject* Player::GetNetworkObject(){ return (networkObject); }

vector3df Player::GetVelocity(){
	vector3df toRet = vector3df(-999,-999,-999);
	if(hasCharacter) toRet = bt_body->GetLinearVelocity();
	return toRet;
}

Kinematic Player::GetKinematic(){
	Kinematic cKin;
	cKin.position = GetPos();
	cKin.orientation =  vector2df(GetRot());
   	cKin.velocity = GetVelocity();
    cKin.rotation = vector2df(GetAngularVelocity());
    return cKin;
}

vector3df Player::GetHeadPos(){
	float offset = -0.1;
	vector3df headPos = m_position;
	vector3df cameraRot = GetRot();

	headPos.X += sin(cameraRot.Y) * offset;
	headPos.Y += 0.5; // Y OFFSET
	headPos.Z += cos(cameraRot.Y) * offset;

	return (headPos);
}

int Player::GetNumberSpells(){ return numberSpells; }

bool Player::GetReadyStatus(){ return readyToStart; }

Alliance Player::GetAlliance(){ return playerAlliance; }

PlayerController* Player::GetController(){
	return controller;
}

void Player::SetAlliance(Alliance newAlliance){

	if(newAlliance == ERR_ALLIANCE) return;

	playerAlliance = newAlliance;
	PlayerManager::GetInstance()->ChangeAlliance(newAlliance, this);

	switch(newAlliance){
		case(ALLIANCE_WIZARD):{
			if(hasCharacter){
				m_playerNode->Remove();
				if(isPlayerOne) m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/WizardArm.obj");
			else m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/Wizard.obj");
				m_playerNode->setMaterialTexture(0, "./../assets/textures/Wizard.png");
				m_playerNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
			}
			if(isPlayerOne && networkObject != NULL) networkObject->SetIntVar(PLAYER_ALLIANCE, ALLIANCE_WIZARD, true, false);
			break;
		}
		case(ALLIANCE_WARLOCK):{
			if(hasCharacter){
				m_playerNode->Remove();
				if(isPlayerOne) m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/WarlockArm.obj");
			else m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/Warlock.obj");
				m_playerNode->setMaterialTexture(0, "./../assets/textures/Warlock.png");
				m_playerNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
			}
			if(isPlayerOne && networkObject != NULL) networkObject->SetIntVar(PLAYER_ALLIANCE, ALLIANCE_WARLOCK, true, false);
			break;
		}
		default:{
			if(hasCharacter){
				m_playerNode->Remove();
				if(isPlayerOne) m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/WizardArm.obj");
			else m_playerNode = engine->addObjMeshSceneNode("./../assets/modelos/npc.obj");
				m_playerNode->setMaterialTexture(0, "./../assets/textures/npc.png");
				m_playerNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
			}
			if(isPlayerOne && networkObject != NULL) networkObject->SetIntVar(PLAYER_ALLIANCE, NO_ALLIANCE, true, false);
			break;
		}
	}
}

void Player::SetPosition(vector3df pos){
	if(hasCharacter){
		m_position = pos;
		m_playerNode->setPosition(pos);
		m_playerNode->updateAbsolutePosition();
		bt_body->SetPosition(pos);
	}
}

void Player::SetPosX(float posX){
	if(hasCharacter){
		m_position.X = posX;
		m_playerNode->setPosition(m_position);
	}
}

void Player::SetPosY(float posY){
	if(hasCharacter){
		m_position.Y = posY;
		m_playerNode->setPosition(m_position);
	}
}

void Player::SetRotation(vector3df rotation){
	if(hasCharacter){
		this->rotation = rotation;
		vector3df newRot = this->rotation;
		newRot.X = 0; newRot.Z = 0;
		newRot = newRot * 180 / M_PI;
		m_playerNode->setRotation(newRot);
		bt_body->SetRotation(newRot);
	}
}

/*	This method makes the graphic body rotate on X axis if is player one
void Player::SetRotation(vector3df rotation){
	if(hasCharacter){
		vector3df newRotGraphic = this->rotation;
		vector3df newRotPhysic = this->rotation;
		
		newRotGraphic.Z = 0;
		if(!isPlayerOne) newRotGraphic.X = 0;
		newRotGraphic = newRotGraphic * 180 / M_PI;
		m_playerNode->setRotation(newRotGraphic);
		
		newRotPhysic.Z = 0;
		newRotPhysic.X = 0;
		newRotPhysic = newRotPhysic * 180 / M_PI;
		bt_body->SetRotation(newRotPhysic);
	}
}
*/

void Player::SetHP(float HP){ m_HP = HP; }

void Player::SetDamageMult(float damageMultiplier){ m_DamageMult *= damageMultiplier; }

void Player::SetDead(bool flag){ m_dead = flag; }

void Player::SetMaxVelocity(float max){ max_velocity = max; }

void Player::SetNetworkObject(NetworkObject* newNetworkObject){ networkObject = newNetworkObject; }

void Player::SetMatchStatus(bool started){ matchStarted = started; }