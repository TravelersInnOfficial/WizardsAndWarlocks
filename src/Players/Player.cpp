#include <cmath>

#include "./Player.h"
#include <PhysicsEngine/BulletEngine.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../AI/SenseManager/RegionalSenseManager.h"

#include <SpellCodes.h>
#include <TrapCodes.h>
#include <Constants.h>
#include "./../Objects/Potion.h"

#include "./../Cameras/WatcherCamera.h"
#include "./../Cameras/FPSCamera.h"

GraphicEngine* engine;

Player::Player(bool isPlayer1){
	// Inicializamos la variable global
	engine = GraphicEngine::getInstance();

	if(isPlayer1) overlayManager = new OverlayManager();
	else overlayManager = nullptr;

	createSoundEvents();
	changeSurface(2);
	m_position = vector3df(0,0,0);
	m_dimensions = vector3df(1.8,1.8,1.8);

	controller = new PlayerController();
	DeclareInput();

	raycastDistance = 2.0f;
	max_velocity = 3.0f;
	max_velocityY = 6.0f;

	playerAlliance = NO_ALLIANCE;
	isPlayerOne = isPlayer1;
	clase = EENUM_PLAYER;

	bt_body = nullptr;
	m_playerNode = nullptr;
	m_camera = nullptr;
	networkObject = nullptr;
	targetDeadCam = nullptr;

	canJump = false;
	matchStarted = false;
	hasCharacter = false;
	readyToStart = false;
	moving = false;
	stepsStarted = false;
	pulseStarted = false;
	isRunning = false;

	currentJumpCheckTime = .05f;
	maxJumpCheckTime = .05f;
	CheckIfCanJump(0, true);

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

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isClient = n_engine->IsClientInit();
	bool isServer = n_engine->IsServerInit();
	if(!isClient && !isServer){ 
		SetRandomName();	// Hace falta que el player ya este creado para poner el billboard
	}

	Respawn();
}

void Player::InitGame(){
	Respawn();
}

void Player::RestartMatchStatus(){
	Respawn();
}

void Player::PlayerInit(){
	potion = nullptr;
	m_HP = 100;
	m_MP = 100;
	m_SP = 100;
	m_DamageMult = 1;
	m_Defense = 1;
	m_shotEffect = WEAK_BASIC;
	m_visible = true;
	m_dead = false;
	canJump = false;

	if(overlayManager != nullptr){
		overlayManager->SetTime(BLOOD, 0);
		overlayManager->SetTime(HITLANDED, 0);
		overlayManager->SetTime(FUZZY, 0);
	}

	TrapManager::GetInstance()->setPlayerUsings(this, 4);
	EffectManager::GetInstance()->CleanEffects(this);
	stopPulse();
}

Player::~Player(){

	delete controller;
	delete overlayManager;

	if(bt_body != nullptr){
		delete bt_body;
		bt_body = nullptr;
	}

    if(m_playerNode != nullptr){
		delete m_playerNode;
		m_playerNode = nullptr;
	}

	if(m_camera!=nullptr){
		delete m_camera;
		m_camera = nullptr;
	}

	std::map<std::string, SoundEvent*>::iterator it = soundEvents.begin();
	for(; it!=soundEvents.end(); it++){
		SoundEvent* even = it->second;
		if (even->isPlaying()) even->stop();	//Stop the sound if its playing
		even->release();
		delete even;						//Release the sound
	}

	TrapManager::GetInstance()->ErasePlayer(this);	
	SpellManager::GetInstance()->ErasePlayer(this);
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

		// Camera
		if(isPlayerOne){ 
			if(m_camera!=nullptr) delete m_camera;
			m_camera = new FPSCamera(120.0f, 0);		
		}

		hasCharacter = true;
	}
}

/**
 * @brief Destoys player visual body only when game has started
 * 
 */
void Player::DestroyPlayerCharacter(){
	if(bt_body != nullptr){
		delete bt_body;
		bt_body = nullptr;
	}

    if(m_playerNode != nullptr){
		delete m_playerNode;
		m_playerNode = nullptr;
	}
	
	if(isPlayerOne && m_camera!=nullptr){
		delete m_camera;
		m_camera = new WatcherCamera(GetPos());
	}
	
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
	if(isPlayerOne && networkObject != nullptr) {
		networkObject->SetIntVar(PLAYER_SET_ALL_INPUT, (keyStatesENUM)state, true, false);
	}
}

void Player::UpdateInput(){
	controller->UpdateOwnStatus();
	if(isPlayerOne) controller->Update();
}

void Player::CheckInput(){}

void Player::GetNetInput(){

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();

	// El server no debe leerlo
	if(!isServer){
		bool doRespawn = networkObject->GetBoolVar(PLAYER_RESPAWN);
		if(doRespawn){
			Respawn();
			doRespawn = false;
			networkObject->SetBoolVar(PLAYER_RESPAWN, doRespawn, false, false);
		}
	}

	// Solo el server debe leerlo
	/*if(isServer){
		bool isReady = networkObject->GetBoolVar(PLAYER_READY);
		readyToStart = isReady;
	}*/

	// Tanto el server como el cliente deben leerlo
	// Pero solo si no es el PLAYER ONE
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

		std::string auxName = networkObject->GetStringVar(PLAYER_NAME);
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

void Player::DeadUpdate(){
	if(isPlayerOne){
		PlayerManager* playerManager = PlayerManager::GetInstance(); // GetPos

		UpdateInput();

		Player* newP = targetDeadCam;
		bool setPos = false;
		if(controller->IsKeyPressed(ACTION_SHOOT)){ 
			newP = playerManager->ChangePlayerTargetCam(targetDeadCam, playerAlliance);
			setPos = true;
		}
		else if(!playerManager->PlayerAlive(targetDeadCam)){
			newP = playerManager->ChangePlayerTargetCam(targetDeadCam, playerAlliance);
			setPos = true;
		}

		if(setPos && newP!=nullptr){
			targetDeadCam = newP;
			m_camera->SetPosition(targetDeadCam->GetPos());
		}
		if(targetDeadCam!=nullptr) m_camera->UpdateCamera(targetDeadCam->GetPos());
	}
}

void Player::Update(float deltaTime){
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
		CheckIfCanJump(deltaTime);		// Comprobamos si podemos saltar
		UpdateSP(deltaTime);			// Updateamos SP (sumamos o restamos segun isRunning)

		// En el caso de que se estuviera moviendo en el frame anterior cambiamos la variable
		if(moving){
			if(!stepsStarted && canJump) playFootsteps();
			moving = false;
		}

		// Si no se estaba moviendo lo frenamos
		else{
			if(stepsStarted) stopFootsteps();
			vector3df velocity = bt_body->GetLinearVelocity();
			bt_body->SetLinearVelocity(vector3df(velocity.X/1.5, velocity.Y, velocity.Z/1.5));
		}

		CheckInput(); // Comprobamos los Input del personaje

		// Actualizamos el cuerpo visual del personaje respecto al fisico
		UpdatePosShape();
		UpdateSoundsPosition();

		// En el caso de que sea el jugador 1 actualizamos su camara
		if(isPlayerOne && m_camera != nullptr){
			vector3df newRot = m_camera->GetRotation();
			vector3df rot = newRot * M_PI / 180.0;	
			SetRotation(rot);

			//Position camera FPS Y TPS
			m_camera->UpdateCamera(GetHeadPos());
		}

		// Comprobamos la velocidad maxima del jugador para que no se sobrepase
		checkMaxVelocity();

		if(overlayManager!=nullptr) overlayManager->Update(deltaTime);
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
}

void Player::checkMaxVelocity(){
	if(hasCharacter){
		vector3df velocity = bt_body->GetLinearVelocity();
		
		// Fix velocity HORIZONTAL
		vector3df auxVelocityH(velocity.X,0,velocity.Z);
		float speedH = auxVelocityH.length();
		if(speedH > max_velocity) {
			auxVelocityH.X *= max_velocity/speedH;
			auxVelocityH.setY(velocity.Y);
			auxVelocityH.Z *= max_velocity/speedH;
			bt_body->SetLinearVelocity(auxVelocityH);
		}

		// Fix velocity VERTICAL
		vector3df auxVelocityV(0, velocity.Y, 0);
		float speedV = auxVelocityV.length();
		if(speedV > max_velocityY) {
			auxVelocityV.setX(auxVelocityH.X);
			auxVelocityV.Y *= max_velocityY/speedV;
			auxVelocityV.setZ(auxVelocityH.Z);
			bt_body->SetLinearVelocity(auxVelocityV);
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
		vector3df rot = rotation;
		bt_body->ApplyCentralImpulse(vector3df(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y)));
		moving = true;
	}
}

void Player::MoveZ(int dir){
	if(hasCharacter){
		float impulse = 30;
		impulse *= dir;
		vector3df rot = rotation;
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
		currentJumpCheckTime = maxJumpCheckTime;
	}
}

void Player::ChangeHP(float HP){

	// SERVIDOR
	if(networkObject != nullptr){
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		bool isServer = n_engine->IsServerInit();
		if(isServer){
			if(HP < 0) SetController(ACTION_RAYCAST, RELEASED);
			m_HP += HP / m_Defense;
		}
	}

	// UN JUGADOR
	else{
		if (HP < 0) {
			if (m_HP + HP > 0) playSoundEvent(soundEvents["hit"]); //We want to play while its alive but not when it dies
			if(overlayManager != nullptr) overlayManager->SetTime(BLOOD, 1);
			SetController(ACTION_RAYCAST, RELEASED);
		}

		// Solo le aplica danyo si su armadura es inferior a 5
		if(m_Defense<5.0f) m_HP += HP / m_Defense;
		else if(overlayManager != nullptr)  overlayManager->SetTime(BLOOD, 0);
	}
	
	// AMBOS
	if(m_HP >= 100) m_HP = 100;
	else if(m_HP <= 0){
		m_HP = 0;
		m_dead = true;
		if(overlayManager != nullptr) overlayManager->SetTime(BLOOD, 0);
	}
}

void Player::SetController(ACTION_ENUM action, keyStatesENUM state){
	controller->SetStatus(action, state);
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

void Player::UpdateSP(float deltaTime){
	float useCost = 30*deltaTime;	// 30 = Consumo en 1 segundo

	if(isRunning && moving) m_SP -= useCost;
	else m_SP += (useCost/2);
	
	if(m_SP <= 0){
		m_SP = 0;
		Run(false);
	}
	else if (m_SP > 100) m_SP = 100;
}

void Player::Respawn(){
	if(isPlayerOne){ 
		MenuType * current_menu = MenuManager::GetInstance()->GetCurrentMenu();
		if(current_menu != nullptr && *current_menu != ENDMATCH_M ) MenuManager::GetInstance()->ClearMenu();
	}

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();
	if(isServer && networkObject != nullptr){
		networkObject->SetBoolVar(PLAYER_RESPAWN, true, true, false);
		networkObject->SetBoolVar(PLAYER_RESPAWN, false, false, false);
	}

	PlayerManager::GetInstance()->AddToLife(this);
	CreatePlayerCharacter();
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
	if(Object!=nullptr){
		Entidad* h = (Entidad*)Object;
		h->Interact(this);
	}
}

bool Player::StartSpell(){
	// Get the code of the currentSpell
	SPELLCODE code = SpellManager::GetInstance()->GetSpellCode(currentSpell, this);
	EffectManager* effectman = EffectManager::GetInstance();
	if((effectman->CheckEffect(this, WEAK_SILENCED) && code!=SPELL_PROJECTILE && code!=SPELL_CLEANSE)
	 || !SpellManager::GetInstance()->StartHechizo(currentSpell, this)){		// if is not a basic spell or if silenced then not shoot
		playSoundEvent(soundEvents["nomana"]);
		return false;
	}

	

	return SpellManager::GetInstance()->StartHechizo(currentSpell,this);
}

bool Player::ShootSpell(){
	// Get the code of the currentSpell
	SPELLCODE code = SpellManager::GetInstance()->GetSpellCode(currentSpell, this);
	EffectManager* effectman = EffectManager::GetInstance();
	
	if(effectman->CheckEffect(this, WEAK_SILENCED) && code!=SPELL_PROJECTILE && code!=SPELL_CLEANSE){		// if is not a basic spell or if silenced then not shoot
		ResetSpell();
		return false;
	}

	SetController(ACTION_RAYCAST, RELEASED);
	return SpellManager::GetInstance()->LanzarHechizo(currentSpell,this);
}

void Player::ResetAllSpells(){
	SpellManager::GetInstance()->ResetHechizo(this);
}

void Player::ResetDieSpells(){
	SpellManager::GetInstance()->ResetDieHechizo(this);
}

void Player::ResetSpell(){
	SpellManager::GetInstance()->ResetHechizo(currentSpell,this);
}

void Player::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();

	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	if(m_visible) sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+playerAlliance), 5.0f, GetKinematic(), AI_SIGHT);
	if(moving) sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+playerAlliance), 5.0f, GetKinematic(), AI_HEARING);
}

void Player::Die(){
	GUIEngine::GetInstance()->ShowDeathMessage(name);
	ObjectManager::GetInstance()->AddPlayerParts(playerAlliance, m_position, m_dimensions, rotation);

	ResetDieSpells();										// Reseteamos los hechizos del jugador

	stopPulse();											// Stop the pulse event
	playSoundEvent(soundEvents["die"]); 												// Play the sound event
	DropObject();											// Soltamos los objetos que teniamos

	PlayerManager::GetInstance()->AddToDead(this);			// Lo anyadimos a la lista de muertos		
	if(matchStarted) DestroyPlayerCharacter();				// Destruimos cuerpo fisico
}

void Player::ReturnToLobby(){

	if(networkObject == nullptr) Respawn();

	else if(networkObject != nullptr){
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		bool isServer = n_engine->IsServerInit();

		if(isPlayerOne) networkObject->SetBoolVar(PLAYER_READY, false, true, false);
		else if(isServer) Respawn();
	}

}

void Player::DrawOverlays(){
	if(overlayManager != nullptr && isPlayerOne) overlayManager->Draw();	
}

bool Player::CheckIfReady(){
	readyToStart = false;

	if(hasCharacter){
		vector4df readyZone = ObjectManager::GetInstance()->GetReadyZone();

		bool ready = true;

		if(		m_position.X < readyZone.X
			|| 	m_position.X > readyZone.X2
			|| 	m_position.Z < readyZone.Y
			|| 	m_position.Z > readyZone.Y2){
			ready = false;	
		}

		readyToStart = ready;
	}

	return readyToStart;
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
	if(potion!=nullptr){
		potion->CreatePotion(m_position, vector3df(0,0,0));
		potion = nullptr;
	}
}

void Player::LosePotion(){
	if(potion!=nullptr) potion = nullptr;
	playSoundEvent(soundEvents["losepotion"]);
}

void Player::UseObject(){
	if(potion!=nullptr){
		playSoundEvent(soundEvents["drink"]);
		potion->Use(this);
		potion = nullptr;
	}
}

bool Player::HasObject(){
	if(potion!=nullptr){
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
	if(Object!=nullptr){
		Entidad* h = (Entidad*)Object;
		if(h->GetClase() == EENUM_FLOOR){
			bool putTrap = false;
			
			if(networkObject == nullptr) putTrap = true;
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
	if(overlayManager != nullptr) overlayManager->SetTime(HITLANDED, 0.205);
}

void Player::ApplyFuzyEffect(){
	if(overlayManager != nullptr) overlayManager->SetTime(FUZZY, 5.0f);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
 
void Player::createSoundEvents() {
	//Create the events
	SoundEvent * footsteps  = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Footsteps");
	SoundEvent * drink 	    = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Drink");
	SoundEvent * die        = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Die");
	SoundEvent * hit        = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Hit");
	SoundEvent * pulse      = SoundSystem::getInstance()->createEvent("event:/Character/Hard/Pulse");
	SoundEvent * losepotion = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Caronte Taxes");
	SoundEvent * nomana		= SoundSystem::getInstance()->createEvent("event:/HUD/Spell Disabled");

	//Store them at the player's sounds map
	soundEvents["footsteps"]  = footsteps;
	soundEvents["drink"] 	  = drink;
	soundEvents["die"] 		  = die;
	soundEvents["hit"] 		  = hit;
	soundEvents["pulse"]      = pulse;
	soundEvents["losepotion"] = losepotion;
	soundEvents["nomana"] 	  = nomana;
}

void Player::playFootsteps() {
	stepsStarted = true;
	SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["footsteps"], GetPos());
}

void Player::playPulse() {
	if(isPlayerOne){
		pulseStarted = true;
		SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["pulse"],GetPos());
	}
}

void Player::playSoundEvent(SoundEvent* event) {
	SoundSystem::getInstance()->playEvent(event, GetPos());
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
		if (soundEvents["footsteps"] != nullptr) soundEvents["footsteps"]->setPosition(GetHeadPos());
	}
}

void Player::changeSurface(float n) {
	if (soundEvents["footsteps"] != nullptr) soundEvents["footsteps"]->setParamValue("Surface", n);
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
	cKin.position = GetHeadPos();
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
			if(isPlayerOne && networkObject != nullptr) networkObject->SetIntVar(PLAYER_ALLIANCE, ALLIANCE_WIZARD, true, false);
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
			if(isPlayerOne && networkObject != nullptr) networkObject->SetIntVar(PLAYER_ALLIANCE, ALLIANCE_WARLOCK, true, false);
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
			if(isPlayerOne && networkObject != nullptr) networkObject->SetIntVar(PLAYER_ALLIANCE, NO_ALLIANCE, true, false);
			break;
		}
	}

	m_HP = 100;
	m_MP = 100;
	m_SP = 100;
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

void Player::SetRotation(vector3df rot){
	if(hasCharacter){
		rotation = rot;
		vector3df newRot = rotation;
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
		if(networkObject != nullptr && isPlayerOne) networkObject->SetStringVar(PLAYER_NAME, name, true, false);
		else if(!isPlayerOne) SetBillboard();
	}
}

void Player::SetVisible(bool visible){
	if(!visible) {
		m_visible = false;
		m_playerNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	}
	else{
		m_visible = true;
	 	m_playerNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	}
}

void Player::SetBillboard(){
	if(!isPlayerOne){
		m_playerNode->AddText(name, vector3df(0,1.25f,0), -1);
	}
}

void Player::Draw(){
	if(m_dead && targetDeadCam!=nullptr){
		targetDeadCam->Draw();
	}else{	
		DrawOverlays();
		DrawBars();
		DrawSpellSelector();
		DrawInventory();
		DrawTraps();
	}
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
	if(potion != nullptr) potion->DrawHUD();
}

void Player::DrawTraps(){
	if(playerAlliance == ALLIANCE_WARLOCK) TrapManager::GetInstance()->DrawHUD(this);
}

bool Player::CheckIfCanJump(float deltaTime, bool forceSkip){
	if(!forceSkip) currentJumpCheckTime -= deltaTime;
	
	if(forceSkip || currentJumpCheckTime <= 0){
		canJump = JumpRaycast();
		currentJumpCheckTime = maxJumpCheckTime;
	}

	return canJump;
}

bool Player::JumpRaycast(){
	bool auxCanJump = false;
	float bodyLength = 1.5f;
	float halfSize = 1.8f * 0.15f;
	float hipotenuse = sqrtf(powf(halfSize, 2) + powf(halfSize, 2));

	// Centro
	vector3df startCenter = GetHeadPos();
	vector3df endCenter(startCenter.X, startCenter.Y - bodyLength, startCenter.Z);
	void* Object = BulletEngine::GetInstance()->Raycast(startCenter, endCenter);
	if(Object != nullptr) auxCanJump = true;
	
	// Esquinas
	float headRotation = GetRotY();
	for(int i = 0; i < 8 && !auxCanJump; i++){
		float offsetX = 0;
		float offsetZ = 0;

		if(i % 2 == 0){
			offsetX = startCenter.X + sin(headRotation) * halfSize;
			offsetZ = startCenter.Z + cos(headRotation) * halfSize;
		}

		else{
			offsetX = startCenter.X + sin(headRotation) * hipotenuse;
			offsetZ = startCenter.Z + cos(headRotation) * hipotenuse;
		}

		vector3df cornerStart = vector3df(offsetX, startCenter.Y, offsetZ);
		vector3df cornerEnd = vector3df(offsetX, startCenter.Y - bodyLength, offsetZ);

		void* Object = BulletEngine::GetInstance()->Raycast(cornerStart, cornerEnd);
		if(Object != nullptr) auxCanJump = true;
		headRotation += M_PI/4.0f;
	}

	return auxCanJump;
}

bool Player::IsDead(){
	return m_dead;
}

void Player::SetRandomName(){
	int arraySize = sizeof(defaultNames)/sizeof(defaultNames[0]);
	int index = rand() % arraySize;
	std::string auxName = defaultNames[index];
	SetName(auxName);
}