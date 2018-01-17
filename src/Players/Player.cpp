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

	createSoundEvents();
	changeSurface(2);
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
	moving = false;
	stepsStarted = false;
	pulseStarted = false;
	isRunning = false;

	name = "";

	currentSpell = 0;
	numberSpells = 3;   // Rango de hechizos [0 a numberSpells]

	SpellManager* spellManager = SpellManager::GetInstance();
	spellManager->AddHechizo(0, this, SPELL_PROJECTILE);
	spellManager->AddHechizo(1, this, SPELL_FIRE);
	spellManager->AddHechizo(2, this, SPELL_WALL);
	spellManager->AddHechizo(3, this, SPELL_BLIZZARD);

	TrapManager::GetInstance()->AddTrapToPlayer(this, TENUM_EXPLOSIVE);
	CreatePlayerCharacter(true);
	Respawn();
}

void Player::PlayerInit(){
	potion = NULL;
	m_HP = 100;
	m_MP = 100;
	m_SP = 100;
	m_DamageMult = 1;
	m_Defense = 1;
	m_shotEffect = WEAK_BASIC;
	m_dead = false;
	bloodOverlayTime = 0;
	hitOverlayTime = 0;
	fuzzyOverlayTime = 0;
	TrapManager::GetInstance()->setPlayerUsings(this, 4);
	EffectManager::GetInstance()->CleanEffects(this);
	stopPulse();
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

		SetBillboard();

		// Physic Player
		vector3df HalfExtents(m_dimensions.X * 0.15f, m_dimensions.Y * 0.45, m_dimensions.Z * 0.15f);
		bt_body = new BT_Body();
		bt_body->CreateBox(m_position, HalfExtents, 50, 2.3, vector3df(0,0,0),C_PLAYER, playerCW);
		bt_body->AssignPointer(this);

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

	CheckIfReady();
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
	controller->AddAction(KEY_KEY_R, ACTION_USE_OBJECT);
	controller->AddAction(KEY_KEY_T, ACTION_DROP_OBJECT);
	controller->AddAction(KEY_LBUTTON, ACTION_SHOOT);
	controller->AddAction(KEY_KEY_F, ACTION_DEPLOY_TRAP);
	controller->AddAction(KEY_WHEEL_UP, ACTION_CHANGE_SPELL_UP);
	controller->AddAction(KEY_WHEEL_DOWN, ACTION_CHANGE_SPELL_DOWN);
	controller->AddAction(KEY_KEY_1, ACTION_SELECT_SPELL_00);
	controller->AddAction(KEY_KEY_2, ACTION_SELECT_SPELL_01);
	controller->AddAction(KEY_KEY_3, ACTION_SELECT_SPELL_02);
	controller->AddAction(KEY_KEY_4, ACTION_SELECT_SPELL_03);
	controller->AddAction(KEY_LSHIFT, ACTION_RUN);
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

	if(!isPlayerOne){
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

		string auxName = networkObject->GetStringVar(PLAYER_NAME);
		if(auxName.length() > 0){
			SetName(auxName);
			auxName = "";
			networkObject->SetStringVar(PLAYER_NAME, auxName, false, false);
		}
	}

}

void Player::SetNetInput(){
}

void Player::RefreshServer(){
	networkObject->SetIntVar(PLAYER_ALLIANCE, playerAlliance, true, false);
	networkObject->SetFloatVar(PLAYER_LIFE, m_HP, true, false);
	networkObject->SetFloatVar(PLAYER_MANA, m_MP, true, false);
	networkObject->SetStringVar(PLAYER_NAME, name, true, false);
}

void Player::Update(){

	// Actualizamos el HP con 0 para comprobar la muerte
	ChangeHP(0);

	// En el caso de que se cumpla alguna de las condiciones de muerte lo matamos
	if((m_dead || m_position.Y < -50) && hasCharacter) Die();

	// Controlamos el pulse
	if (m_HP <= 20 && m_HP > 0) {
		soundEvents["pulse"]->setParamValue("Life", m_HP);
		if (!pulseStarted) playPulse();
	}
	else if (pulseStarted) stopPulse();

	// Si tenemos cuerpo fisico
	if(hasCharacter){
		vector3df velocity = bt_body->GetLinearVelocity();
		if(!canJump){
			float verticalSpeed = velocity.Y;
			float offsetSpeed = fabs(lastVerticalSpeed - verticalSpeed);
			if(fabs(verticalSpeed < 0.1) && offsetSpeed < 0.1) canJump = true;
			lastVerticalSpeed = verticalSpeed;
		}

		UpdateSP();	// Updateamos SP (sumamos o restamos segun isRunning)

		// En el caso de que se estuviera moviendo en el frame anterior cambiamos la variable, mientras
		// que si no se estaba moviendo lo frenamos
		if(moving){
			if(!stepsStarted && canJump) playFootsteps();
			moving = false;
		}
		else{
			if(stepsStarted) stopFootsteps();
			bt_body->SetLinearVelocity(vector3df(velocity.X/1.5, velocity.Y, velocity.Z/1.5));
		}

		// Comprobamos los Input del personaje
		CheckInput();

		// Actualizamos el cuerpo visual del personaje respecto al fisico
		UpdatePosShape();
		UpdateSoundsPosition();

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

bool Player::ChangeCurrentSpell(int value){
	if(currentSpell != value){
		if(value >=0 && value<= numberSpells){
			ResetSpell();
			currentSpell = value;
			return true;
		}
	}
	return false;
}

void Player::SetSpell(int value){
	ChangeCurrentSpell(value);
}

int Player::GetCurrentSpell(){
	return currentSpell;
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
		stopFootsteps();
		vector3df velocity = bt_body->GetLinearVelocity();
		velocity.setY(0);
		float impulse = 30 * 9.8;
		bt_body->ApplyCentralImpulse(vector3df(0,impulse,0));
		m_position.Y = bt_body->GetPosition().Y;
		canJump = false;
	}
}

void Player::ChangeHP(float HP){

	// MULTIJUGADOR
	if(networkObject != NULL){
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		bool isServer = n_engine->IsServerInit();
		if(isServer) m_HP += HP / m_Defense;
	}

	// UN JUGADOR
	else{
		if (HP < 0) {
			if (m_HP + HP > 0) 	playHit(); //We want to play while its alive but not when it dies
			bloodOverlayTime = 1;
		}
		// Solo le aplica danyo si su armadura es inferior a 5
		if(m_Defense<5.0f){
			m_HP += HP / m_Defense;
		}else{
			bloodOverlayTime = 0;
		}
	}
	
	// AMBOS
	if(m_HP >= 100) m_HP = 100;
	else if(m_HP <= 0){
		m_HP = 0;
		m_dead = true;
		bloodOverlayTime = 0;
	}
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

void Player::UpdateSP(){
	float useCost = 0.5;

	if(isRunning && moving) m_SP -= useCost;
	else m_SP += (useCost/2);
	
	if(m_SP <= 0){
		m_SP = 0;
		Run(false);
	}
	else if (m_SP > 100) m_SP = 100;
}

void Player::Respawn(){
	// CreatePlayerCharacter();
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
	EffectManager* effectman = EffectManager::GetInstance();
	if(effectman->CheckEffect(this, WEAK_SILENCED) && currentSpell!=0){		// if is not a basic spell or if silenced then not shoot
		ResetSpell();
		return false;
	}
	return SpellManager::GetInstance()->LanzarHechizo(currentSpell,this);
}

void Player::ResetAllSpells(){
	SpellManager::GetInstance()->ResetHechizo(this);
}

void Player::ResetSpell(){
	SpellManager::GetInstance()->ResetHechizo(currentSpell,this);
}

void Player::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+playerAlliance), 5.0f, GetKinematic(), AI_SIGHT);
	if(moving){
		sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+playerAlliance), 5.0f, GetKinematic(), AI_HEARING);
	}
}

void Player::Die(){

	stopPulse();	//Stop the pulse event
	playDie(); 		//Play the sound event
	DropObject();

	if(matchStarted){
		PlayerManager::GetInstance()->AddToDead(playerAlliance, this);
		DestroyPlayerCharacter();
	}
	
	Respawn();
}

void Player::ReturnToLobby(){
	if(isPlayerOne && networkObject != NULL) CheckIfReady();

	CreatePlayerCharacter();
	Respawn();
	if(networkObject != NULL){
		networkObject->SetBoolVar(PLAYER_CREATE_CHAR, true, true, false);
		networkObject->SetBoolVar(PLAYER_RESPAWN, true, true, false);
	}
}

void Player::DrawOverlays(float deltaTime){
	bloodOverlayTime -= deltaTime;
	hitOverlayTime -= deltaTime;
	fuzzyOverlayTime -= deltaTime;
	
	if(hitOverlayTime > 0) engine->drawOverlays(1);
	if(bloodOverlayTime > 0) engine->drawOverlays(0);
	if(fuzzyOverlayTime > 0) engine->drawOverlays(2);
}

void Player::CheckIfReady(){
	
	if(hasCharacter){
		vector4df readyZone = ObjectManager::GetInstance()->GetReadyZone();

		bool ready = true;
		if(m_position.X < readyZone.X || m_position.X > readyZone.X2 || m_position.Z < readyZone.Y || m_position.Z > readyZone.Y2){
			ready = false;
		}

		readyToStart = ready;
	}
	else readyToStart = false;
	
	if(networkObject != NULL) networkObject->SetBoolVar(PLAYER_READY, readyToStart, true, false);
}

void Player::Run(bool runStatus){
	float factor = 5/3.0f;
	if(isRunning != runStatus){
		isRunning = runStatus;
		if(runStatus) max_velocity *= factor;
		else max_velocity /= factor;
	}
}

void Player::CatchObject(Potion* p){
	DropObject();
	potion = p;
}

void Player::DropObject(){
	if(potion!=NULL){
		potion->CreatePotion(m_position, vector3df(0,0,0));
		potion = NULL;
	}
}

void Player::LosePotion(){
	if(potion!=NULL) potion = NULL;
}

void Player::UseObject(){
	if(potion!=NULL){
		playDrink();
		potion->Use(this);
		ObjectManager::GetInstance()->DeletePotion(potion);
		potion = NULL;
	}
}

bool Player::HasObject(){
	if(potion!=NULL){
		return true;
	}
	return false;
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
		if(h->GetClase() == EENUM_FLOOR){
			bool putTrap = false;
			
			if(networkObject == NULL) putTrap = true;
			else{
				NetworkEngine* n_engine = NetworkEngine::GetInstance();
				bool isServer = n_engine->IsServerInit();
				if(isServer) putTrap = true;
			}

			if(putTrap) TrapManager::GetInstance()->PlayerDeployTrap(this,Start,End);
		}
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

void Player::HitMade(Player* player){
	hitOverlayTime = 0.25f;
}

void Player::ApplyFuzyEffect(){
	fuzzyOverlayTime = 5.0f;
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
 
void Player::createSoundEvents() {
	//Create the events
	SoundEvent * footsteps = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Footsteps");
	SoundEvent * drink 	   = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Drink");
	SoundEvent * die       = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Die");
	SoundEvent * hit       = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Hit");
	SoundEvent * pulse     = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Pulse");
	
	//Store them at the player's sounds map
	soundEvents["footsteps"] = footsteps;
	soundEvents["drink"] 	 = drink;
	soundEvents["die"] 		 = die;
	soundEvents["hit"] 		 = hit;
	soundEvents["pulse"]     = pulse;
}

void Player::playFootsteps() {
	stepsStarted = true;
	SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["footsteps"], GetPos());
}

void Player::playDrink() {
	SoundSystem::getInstance()->playEvent(soundEvents["drink"], GetPos());
}

void Player::playDie() {
	SoundSystem::getInstance()->playEvent(soundEvents["die"], GetPos());
}

void Player::playHit() {
	SoundSystem::getInstance()->playEvent(soundEvents["hit"], GetPos());
}

void Player::playPulse() {
	if(isPlayerOne){
		pulseStarted = true;
		SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["pulse"],GetPos());
	}
}

void Player::stopFootsteps() {
	stepsStarted = false;
	SoundSystem::getInstance()->stopEvent(soundEvents["footsteps"]);
}

void Player::stopPulse() {
	if(isPlayerOne) {
		pulseStarted = false;
		SoundSystem::getInstance()->stopEvent(soundEvents["pulse"]);
	}
}

//Update the event positions for continuous events or usable while moving events (like spells)
void Player::UpdateSoundsPosition(){
	if(stepsStarted){
		if (soundEvents["footsteps"] != NULL) {
			//Update footsteps
			soundEvents["footsteps"]->setPosition(GetHeadPos());
			
			//Also the spells
			
		}
	}
}

void Player::changeSurface(float n) {
	if (soundEvents["footsteps"] != NULL) {
		soundEvents["footsteps"]->setParamValue("Surface", n);
	}
}
/********************************************************************************************************
 ********************************************** GETERS **************************************************
 ********************************************************************************************************/


vector3df Player::GetAngularVelocity(){
	vector3df toRet = vector3df(-999,-999,-999);
	if(hasCharacter) toRet = bt_body->GetAngularVelocity();
	return toRet;
}

vector3df Player::GetPos(){ return m_position; }

float Player::GetRotY(){ return rotation.Y; }

vector3df Player::GetRot(){ return rotation; }

float Player::GetWidth(){ return m_dimensions.X; }

float Player::GetHeight(){ return m_dimensions.Y; }

float Player::GetLength(){ return m_dimensions.Z; }

float Player::GetHP(){ return m_HP; }

float Player::GetMP(){ return m_MP; }

float Player::GetSP(){ return m_SP; }

float Player::GetDamageM(){ return m_DamageMult; }

NetworkObject* Player::GetNetworkObject(){ return (networkObject); }

Potion* Player::GetPotion(){ return potion; }

bool Player::GetHasCharacter(){ return hasCharacter; }

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

std::string Player::GetName(){ return name; }

bool Player::GetMoving(){
	return moving;
}

void Player::SetAlliance(Alliance newAlliance){

	if(newAlliance == ERR_ALLIANCE) return;

	playerAlliance = newAlliance;

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
			TrapManager::GetInstance()->setPlayerUsings(this, 0);
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
			TrapManager::GetInstance()->setPlayerUsings(this, 4);
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

	SetBillboard();
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

void Player::SetHP(float HP){ m_HP = HP; }

void Player::SetSP(float SP){ m_SP = SP; }

void Player::SetDead(bool flag){ m_dead = flag; }

void Player::SetNetworkObject(NetworkObject* newNetworkObject){ networkObject = newNetworkObject; }

void Player::SetMatchStatus(bool started){ matchStarted = started; }

void Player::SetName(std::string newName){
	name = newName;
	if(!name.empty()){
		if(isPlayerOne) networkObject->SetStringVar(PLAYER_NAME, name, true, false);
		else SetBillboard();
	}
}

void Player::SetBillboard(){
	if(!isPlayerOne) m_playerNode->AddText(name, vector3df(0,1.25f,0), -1);
}

void Player::Draw(){
	DrawBars();
	DrawSpellSelector();
	DrawInventory();
	DrawTraps();
}

void Player::DrawBars(){
	int W = engine->GetScreenWidth();		
	int H = engine->GetScreenHeight();

	float size = 20.0f;			// Height of the bar

	float xInit = W/20.0f;		// Calculate the Init and End of the bar on X axis
	float xEnd =  W/3.0f;		

	float yInitH = H * 0.05;	// Calculate the Init of the bar on Y axis
	float yInitM = H * 0.09;
	float yInitS = H * 0.13;

	float yEndH = yInitH + size;	// Calculate the End of the bar on Y axis with the size
	float yEndM = yInitM + size;
	float yEndS = yInitS + size;

	float hP = m_HP/100.0f;		// % of the life
	float mP = m_MP/100.0f;		// % of the mana
	float sP = m_SP/100.0f;		// % of the stamina


	// Black Bar
	vector3df color(0,0,0);
	engine->draw2DRectangle(color, xInit, yInitH, xEnd, yEndH);
	engine->draw2DRectangle(color, xInit, yInitM, xEnd, yEndM);
	engine->draw2DRectangle(color, xInit, yInitS, xEnd, yEndS);
	
	// Helath & Mana Bar
	color = vector3df(255,0,0);
	engine->draw2DRectangle(color, xInit, yInitH, xInit + (xEnd - xInit) * hP, yEndH);
	color = vector3df(0,0,255);
	engine->draw2DRectangle(color, xInit, yInitM, xInit + (xEnd - xInit) * mP, yEndM);
	color = vector3df(255, 255, 0);
	engine->draw2DRectangle(color, xInit, yInitS, xInit + (xEnd - xInit) * sP, yEndS);
}

void Player::DrawSpellSelector(){
	SpellManager::GetInstance()->DrawHUDSpells(this, currentSpell);
}

void Player::DrawInventory(){
	if(potion != NULL) potion->DrawHUD();
}

void Player::DrawTraps(){
	if(playerAlliance == ALLIANCE_WARLOCK) TrapManager::GetInstance()->DrawHUD(this);
}