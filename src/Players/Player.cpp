#include "./Player.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/OverlayManager.h"
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/NetworkObject.h>
#include <GraphicEngine/MenuManager.h>
#include <GraphicEngine/GAnimation.h>
#include <SoundEngine/SoundSystem.h>
#include "./../Managers/ObjectManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Cameras/WatcherCamera.h"
#include <PhysicsEngine/BulletEngine.h>
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Cameras/FPSCamera.h"
#include <PhysicsEngine/BT_Body.h>
#include "./../Spells/Hechizo.h"
#include <GUIEngine/GUIEngine.h>
#include "./../Objects/Potion.h"
#include "./PlayerController.h"
#include <kinematicTypes.h>
#include "./PlayerHUD.h"
#include <SpellCodes.h>
#include <Constants.h>
#include <algorithm>
#include <Assets.h>
#include <Names.h>
#include <cmath>

Player::Player(bool isPlayer1){
	if(isPlayer1) m_overlayManager = new OverlayManager();
	else m_overlayManager = nullptr;

	// Create hud
	m_hud = new PlayerHUD(this);
	
	m_position = vector3df(0,2,0);
	m_dimensions = vector3df(0.9,0.9,0.9);
	m_physicsDimensions = vector3df(0.9 * 0.3, 0.9, 0.9 * 0.3);

	m_controller = new PlayerController();
	DeclareInput();

	m_raycastDistance = 2.75f;
	m_max_velocity = 4.5f;
	m_max_velocityY = 8.0f;

	m_playerAlliance = NO_ALLIANCE;
	m_isPlayerOne = isPlayer1;
	clase = EENUM_PLAYER;

	bt_body = nullptr;
	m_playerNode = nullptr;
	m_playerNodeTop = nullptr;
	m_camera = nullptr;
	m_networkObject = nullptr;
	m_targetDeadCam = nullptr;

	m_CanJump = false;
	m_matchStarted = false;
	m_hasCharacter = false;
	m_readyToStart = false;
	m_moving = false;
	m_stepsStarted = false;
	m_pulseStarted = false;
	m_isRunning = false;

	m_currentJumpCheckTime = .05f;
	m_maxJumpCheckTime = .05f;
	CheckIfCanJump(0, true);

	m_name = "";

	// Spell number
	m_currentSpell = 0;
	m_numberSpells = 3;   // Rango de hechizos [0 a m_numberSpells]

	// Add intial trap type
	TrapManager::GetInstance()->AddTrapToPlayer(this, TENUM_SPIRITS);
	
	// Create visual body and its animations
	CreatePlayerCharacter();

	// Assign random name to player
	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isClient = n_engine->IsClientInit();
	bool isServer = n_engine->IsServerInit();
	if(!isClient && !isServer) SetRandomName();	// Hace falta que el player ya este creado para poner el billboard

	// Create Sounds
	createSoundEvents();
	changeSurface(2);

	// Reposition player
	Respawn();
}

Player::~Player(){

	delete m_controller;
	delete m_overlayManager;

	if(bt_body != nullptr){
		delete bt_body;
		bt_body = nullptr;
	}

	// Delete m_playerNode and m_playerNodeTop
	DestroyPlayerGBody();

	if(m_camera!=nullptr){
		delete m_camera;
		m_camera = nullptr;
	}

	if(m_hud != nullptr){
		delete m_hud;
		m_hud = nullptr;
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

void Player::InitHUD(){
	if(m_hud != nullptr) m_hud->InitHUD();
}

void Player::AddToMinimap(Player* p){
	//aqui solo llega si es player one
	if(m_hud!= nullptr && p!=nullptr) m_hud->ShowEnemyInMap(p);
}

void Player::InitGame(){
	Respawn();
}

void Player::RestartMatchStatus(){
	Respawn();
}

void Player::PlayerInit(){
	m_potion = nullptr;
	m_HP = 100;
	m_MP = 100;
	m_SP = 100;

	if(m_isPlayerOne) InitHUD();

	m_DamageMult = 1;
	m_Defense = 1;
	m_shotEffect = WEAK_BASIC;
	m_visible = true;
	m_dead = false;
	m_CanJump = false;

	if(m_overlayManager != nullptr){
		m_overlayManager->SetTime(BLOOD, 0);
		m_overlayManager->SetTime(HITLANDED, 0);
		m_overlayManager->SetTime(FUZZY, 0);
	}

	TrapManager::GetInstance()->setPlayerUsings(this, 4);
	EffectManager::GetInstance()->CleanEffects(this);
	stopPulse();
}

void Player::InitPlayerAnimations(){
	std::vector<std::string> npc_anims = {"./../assets/modelos/npc.obj"};
	std::vector<std::string> npcArm_anims = {"./../assets/modelos/npc.obj"};

	switch(m_playerAlliance){
		case(ALLIANCE_WIZARD):
			if(m_isPlayerOne){
				// FILL ARM ANIMATION
				m_playerNode->SetPaths("idle", AWIZARD_ARMIDLE);
				m_playerNode->SetPaths("walk", AWIZARD_ARMWALK);
				m_playerNode->SetPaths("run", AWIZARD_ARMRUN);

				m_playerNode->SetPaths("shoot1", AWIZARD_ARMSHOOT1);
				m_playerNode->SetPaths("shoot2", AWIZARD_ARMSHOOT2);
				//m_playerNode->SetPaths("shoot3", AWIZARD_ARMSHOOT3);
				m_playerNode->SetPaths("drink", AWIZARD_ARMDRINK);
				m_playerNode->SetPaths("interact", AWIZARD_ARMINTERACT);
				//m_playerNode->SetPaths("dab", AWIZARD_ARMDAB);
				
				// SET INITIAL LOOP
				m_playerNode->SetAnimationLoop("idle");
			}
			else{
				// FILL WIZARD ANIMATION
				// BASICS
				m_playerNodeTop->SetPaths("idle", AWIZARD_TIDLE);
				m_playerNode->SetPaths("idle",  AWIZARD_BIDLE);

				m_playerNodeTop->SetPaths("walk",  AWIZARD_TWALK);
				m_playerNode->SetPaths("walk",  AWIZARD_BWALK);

				m_playerNodeTop->SetPaths("run",  AWIZARD_TRUN);
				m_playerNode->SetPaths("run",  AWIZARD_BRUN);

				// TOP ONLY
				m_playerNodeTop->SetPaths("shoot1", AWIZARD_TSHOOT1);
				m_playerNodeTop->SetPaths("shoot2", AWIZARD_TSHOOT2);
				m_playerNodeTop->SetPaths("drink", AWIZARD_TDRINK);
				m_playerNodeTop->SetPaths("interact", AWIZARD_TINTERACT);
/*
				m_playerNodeTop->SetPaths("shoot3", AWIZARD_TSHOOT3);
*/

/*
				// JUMP
				m_playerNodeTop->SetPaths("jumpstart",  AWIZARD_TJUMPSTART);
				m_playerNode->SetPaths("jumpstart",  AWIZARD_BJUMPSTART);
				
				m_playerNodeTop->SetPaths("jumpfall",  AWIZARD_TJUMPFALL);
				m_playerNode->SetPaths("jumpfall",  AWIZARD_BJUMPFALL);

				m_playerNodeTop->SetPaths("jumpend",  AWIZARD_TJUMPEND);
				m_playerNode->SetPaths("jumpend",  AWIZARD_BJUMPEND);
*/
/*
				// DANCES
				m_playerNodeTop->SetPaths("circle",  AWIZARD_TCIRCLE);
				m_playerNode->SetPaths("circle",  AWIZARD_BCIRCLE);

				m_playerNodeTop->SetPaths("dab",  AWIZARD_TDAB);
				m_playerNode->SetPaths("dab",  AWIZARD_BDAB);

				m_playerNodeTop->SetPaths("dance",  AWIZARD_TDANCE);
				m_playerNode->SetPaths("dance",  AWIZARD_BDANCE);

				m_playerNodeTop->SetPaths("win",  AWIZARD_TWIN);
				m_playerNode->SetPaths("win",  AWIZARD_BWIN);
*/
				// SET INITIAL LOOP
				m_playerNode->SetAnimationLoop("idle");
				m_playerNodeTop->SetAnimationLoop("idle");
			}
		break;
		
		case(ALLIANCE_WARLOCK):
			if(m_isPlayerOne){
				// FILL ARM ANIMATION
				m_playerNode->SetPaths("idle", AWARLOCK_ARMIDLE);
				m_playerNode->SetPaths("walk", AWARLOCK_ARMWALK);
				m_playerNode->SetPaths("run",  AWARLOCK_ARMRUN);
				
				m_playerNode->SetPaths("shoot1", AWARLOCK_ARMSHOOT1);
				m_playerNode->SetPaths("shoot2", AWIZARD_ARMSHOOT2);
				//m_playerNode->SetPaths("shoot3", AWIZARD_ARMSHOOT3);
				m_playerNode->SetPaths("drink", AWARLOCK_ARMDRINK);
				m_playerNode->SetPaths("interact", AWARLOCK_ARMINTERACT);
				//m_playerNode->SetPaths("dab", AWIZARD_ARMDAB);

				// SET INITIAL LOOP
				m_playerNode->SetAnimationLoop("idle");
			}
			else{
				// FILL WARLOCK ANIMATION
				// BASICS
				m_playerNodeTop->SetPaths("idle", AWARLOCK_TIDLE);
				m_playerNode->SetPaths("idle",  AWARLOCK_BIDLE);

				m_playerNodeTop->SetPaths("walk",  AWARLOCK_TWALK);
				m_playerNode->SetPaths("walk",  AWARLOCK_BWALK);

				m_playerNodeTop->SetPaths("run",  AWARLOCK_TRUN);
				m_playerNode->SetPaths("run",  AWARLOCK_BRUN);

				// TOP ONLY ANIMATIONS
				m_playerNodeTop->SetPaths("shoot1", AWARLOCK_TSHOOT1);
				m_playerNodeTop->SetPaths("shoot2", AWARLOCK_TSHOOT2);
				m_playerNodeTop->SetPaths("drink", AWARLOCK_TDRINK);
				m_playerNodeTop->SetPaths("interact", AWARLOCK_TINTERACT);
/*
				m_playerNodeTop->SetPaths("shoot3", AWARLOCK_TSHOOT3);
*/

/*
				// JUMP
				m_playerNodeTop->SetPaths("jumpstart",  AWARLOCK_TJUMPSTART);
				m_playerNode->SetPaths("jumpstart",  AWARLOCK_BJUMPSTART);

				m_playerNodeTop->SetPaths("jumpfall",  AWARLOCK_TJUMPFALL);
				m_playerNode->SetPaths("jumpfall",  AWARLOCK_BJUMPFALL);

				m_playerNodeTop->SetPaths("jumpend",  AWARLOCK_TJUMPEND);
				m_playerNode->SetPaths("jumpend",  AWARLOCK_BJUMPEND);	
*/

/*				
				// DANCES
				m_playerNodeTop->SetPaths("circle",  AWARLOCK_TCIRCLE);
				m_playerNode->SetPaths("circle",  AWARLOCK_BCIRCLE);

				m_playerNodeTop->SetPaths("dab",  AWARLOCK_TDAB);
				m_playerNode->SetPaths("dab",  AWARLOCK_BDAB);

				m_playerNodeTop->SetPaths("dance",  AWARLOCK_TDANCE);
				m_playerNode->SetPaths("dance",  AWARLOCK_BDANCE);

				m_playerNodeTop->SetPaths("win",  AWARLOCK_TWIN);
				m_playerNode->SetPaths("win",  AWARLOCK_BWIN);
*/
				// SET INITIAL LOOP
				m_playerNode->SetAnimationLoop("idle");
				m_playerNodeTop->SetAnimationLoop("idle");
			}
		break;
		
		default:
			if(m_isPlayerOne){
				m_playerNode->SetPaths("none", AWIZARD_ARMIDLE);
			} 
			else{
				m_playerNode->SetPaths("none", AWIZARD_ARMIDLE);
			} 
			
		break;
	}

	// SYNC ANIMATIONS
	if(!m_isPlayerOne && m_playerNodeTop != nullptr) m_playerNodeTop->BindSyncAnimation(m_playerNode);

}

void Player::CreatePlayerGBody(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	
	// Set initial position of the meshes
	m_playerNode = g_engine->addAnimatedMeshSceneNode(m_position, m_rotation, m_dimensions);

	// Initialize top part
	if(!m_isPlayerOne) 
		m_playerNodeTop = g_engine->addAnimatedMeshSceneNode(m_position, m_rotation, m_dimensions);
	else m_playerNodeTop = nullptr;
	
	// APPLY ALL TEXTURES
	switch(m_playerAlliance){
		case(ALLIANCE_WIZARD):
			m_playerNode->setMaterialTexture(0, "./../assets/textures/Wizard.png");
			if(m_playerNodeTop != nullptr) m_playerNodeTop->setMaterialTexture(0, "./../assets/textures/Wizard.png");
		break;
		
		case(ALLIANCE_WARLOCK):
			m_playerNode->setMaterialTexture(0, "./../assets/textures/Warlock.png");
			if(m_playerNodeTop != nullptr) m_playerNodeTop->setMaterialTexture(0, "./../assets/textures/Warlock.png");
		break;
		default:
			m_playerNode->setMaterialTexture(0, "./../assets/textures/npc.png");
			if(m_playerNodeTop != nullptr) m_playerNodeTop->setMaterialTexture(0, "./../assets/textures/npc.png");
		break;
	}

	// Fill Animation string vectors
	InitPlayerAnimations();
	
}

void Player::CreatePlayerCharacter(){
	if(!m_hasCharacter){
		// Graphic Player
		CreatePlayerGBody();
		
		SetBillboard();

		// Physic Player
		bt_body = new BT_Body();
		bt_body->CreateBox(m_position, m_physicsDimensions, 50, 2.3, vector3df(0,0,0), C_PLAYER, playerCW);
		bt_body->AssignPointer(this);

		// Camera
		if(m_isPlayerOne){
			bool work = true;

			if(m_camera!=nullptr){ 
				work = m_camera->GetWorking();
				delete m_camera;
			}
			m_camera = new FPSCamera(m_position, m_rotation);
			m_camera->SetWorking(work);
		}

		m_hasCharacter = true;
	}
	
}

void Player::DestroyPlayerGBody(){
    if(m_playerNode != nullptr){
		delete m_playerNode;
		m_playerNode = nullptr;
	}
	if(m_playerNodeTop != nullptr){
		delete m_playerNodeTop;
		m_playerNodeTop = nullptr;
	}
}

void Player::DestroyPlayerCharacter(){
	if(bt_body != nullptr){
		delete bt_body;
		bt_body = nullptr;
	}

	// Delete m_playerNode and m_playerNodeTop
	DestroyPlayerGBody();

	if(m_isPlayerOne && m_camera!=nullptr){
		bool work = m_camera->GetWorking();

		delete m_camera;
		m_camera = new WatcherCamera(GetPos());
		m_camera->SetWorking(work);
	}
	
	m_hasCharacter = false;
}

void Player::DeclareInput(){
	m_controller->AddAction(Key_W, ACTION_MOVE_UP);
	m_controller->AddAction(Key_S, ACTION_MOVE_DOWN);
	m_controller->AddAction(Key_A, ACTION_MOVE_LEFT);
	m_controller->AddAction(Key_D, ACTION_MOVE_RIGHT);
	m_controller->AddAction(Key_E, ACTION_RAYCAST);
	m_controller->AddAction(Key_Space, ACTION_JUMP);
	m_controller->AddAction(Key_R, ACTION_USE_OBJECT);
	m_controller->AddAction(Key_T, ACTION_DROP_OBJECT);
	m_controller->AddAction(Key_MouseLeft, ACTION_SHOOT);
	m_controller->AddAction(Key_F, ACTION_DEPLOY_TRAP);
	m_controller->AddAction(Key_MouseMiddle_Up, ACTION_CHANGE_SPELL_UP);
	m_controller->AddAction(Key_MouseMiddle_Down, ACTION_CHANGE_SPELL_DOWN);
	m_controller->AddAction(Key_1, ACTION_SELECT_SPELL_00);
	m_controller->AddAction(Key_2, ACTION_SELECT_SPELL_01);
	m_controller->AddAction(Key_3, ACTION_SELECT_SPELL_02);
	m_controller->AddAction(Key_4, ACTION_SELECT_SPELL_03);
	m_controller->AddAction(Key_LEFT_SHIFT, ACTION_RUN);
	m_controller->AddAction(Key_TAB,ACTION_SHOW_STATUS_MENU);
}

void Player::SetAllInput(keyStatesENUM state){
	m_controller->SetAllStatus(state);
	if(m_isPlayerOne && m_networkObject != nullptr) {
		m_networkObject->SetIntVar(PLAYER_SET_ALL_INPUT, (keyStatesENUM)state, true, false);
	}
}

void Player::UpdateInput(){
	m_controller->UpdateOwnStatus();
	if(m_isPlayerOne) m_controller->Update();
}

void Player::CheckInput(){}

void Player::GetNetInput(){

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();

	// El server no debe leerlo
	if(!isServer){
		bool doRespawn = m_networkObject->GetBoolVar(PLAYER_RESPAWN);
		if(doRespawn){
			Respawn();
			doRespawn = false;
			m_networkObject->SetBoolVar(PLAYER_RESPAWN, doRespawn, false, false);
		}
	}

	// Solo el server debe leerlo
	/*if(isServer){
		bool isReady = m_networkObject->GetBoolVar(PLAYER_READY);
		m_readyToStart = isReady;
	}*/

	// Tanto el server como el cliente deben leerlo
	// Pero solo si no es el PLAYER ONE
	if(!m_isPlayerOne){
		int alliance = m_networkObject->GetIntVar(PLAYER_ALLIANCE);
		if(alliance != (int)NO_ALLIANCE){
			SetAlliance((Alliance)alliance);
			alliance = (int)NO_ALLIANCE;
			m_networkObject->SetIntVar(PLAYER_ALLIANCE, alliance, false, false);
		}

		bool doCreateChar = m_networkObject->GetBoolVar(PLAYER_CREATE_CHAR);
		if(doCreateChar){
			CreatePlayerCharacter();
			doCreateChar = false;
			m_networkObject->SetBoolVar(PLAYER_CREATE_CHAR, doCreateChar, false, false);
		}

		std::string auxName = m_networkObject->GetStringVar(PLAYER_NAME);
		if(auxName.length() > 0){
			SetName(auxName);
			auxName = "";
			m_networkObject->SetStringVar(PLAYER_NAME, auxName, false, false);
		}
	}

}

void Player::SetNetInput(){
}

void Player::RefreshServer(){
	m_networkObject->SetIntVar(PLAYER_ALLIANCE, m_playerAlliance, true, false);
	m_networkObject->SetFloatVar(PLAYER_LIFE, m_HP, true, false);
	m_networkObject->SetFloatVar(PLAYER_MANA, m_MP, true, false);
	m_networkObject->SetStringVar(PLAYER_NAME, m_name, true, false);
}

void Player::DeadUpdate(){
	if(m_isPlayerOne){
		PlayerManager* playerManager = PlayerManager::GetInstance(); // GetPos

		UpdateInput();

		Player* newP = m_targetDeadCam;
		bool setPos = false;
		if(m_controller->IsKeyPressed(ACTION_SHOOT)){ 
			newP = playerManager->ChangePlayerTargetCam(m_targetDeadCam, m_playerAlliance);
			setPos = true;
		}
		else if(!playerManager->PlayerAlive(m_targetDeadCam)){
			newP = playerManager->ChangePlayerTargetCam(m_targetDeadCam, m_playerAlliance);
			setPos = true;
		}

		if(m_targetDeadCam!=newP && newP!=nullptr && m_targetDeadCam!=nullptr){
			eraseTargetHUD();
		}

		if(setPos && newP!=nullptr){
			m_targetDeadCam = newP;
			m_camera->SetPosition(m_targetDeadCam->GetPos());
			m_targetDeadCam->InitHUD();
		}

		if(m_targetDeadCam!=nullptr) m_camera->UpdateCamera(m_targetDeadCam->GetPos());
	}
}

void Player::eraseTargetHUD(){
	if(m_targetDeadCam!=nullptr) m_targetDeadCam->EraseHUD();
}

void Player::Update(float deltaTime){
	// Actualizamos el HP con 0 para comprobar la muerte
	ChangeHP(0);

	// En el caso de que se cumpla alguna de las condiciones de muerte lo matamos
	if((m_dead || m_position.Y < -50) && m_hasCharacter) Die();

	// Controlamos el pulse
	if (m_HP <= 20 && m_HP > 0) {
		soundEvents["pulse"]->setParamValue("Life", m_HP);
		if (!m_pulseStarted) playPulse();
	}
	else if (m_pulseStarted) stopPulse();

	// Si tenemos cuerpo fisico
	if(m_hasCharacter){
		CheckIfCanJump(deltaTime);		// Comprobamos si podemos saltar
		UpdateSP(deltaTime);			// Updateamos SP (sumamos o restamos segun m_isRunning)

		// En el caso de que se estuviera moviendo en el frame anterior cambiamos la variable
		if(m_moving){
			if(!m_stepsStarted && m_CanJump) playFootsteps();
			m_moving = false;
		}
		
		// Si no se estaba moviendo lo frenamos
		else{
			if(m_stepsStarted) stopFootsteps();
			vector3df velocity = bt_body->GetLinearVelocity();
			bt_body->SetLinearVelocity(vector3df(velocity.X/1.5, velocity.Y, velocity.Z/1.5));
		}

		CheckInput(); // Comprobamos los Input del personaje

		// Actualizamos la animacion de idle/andar/correr
		UpdateWalkAnimation();

		// Actualizamos el cuerpo visual del personaje respecto al fisico
		UpdatePosShape(deltaTime);

		// Actualizamos la posicion del sonido
		UpdateSoundsPosition();

		// En el caso de que sea el jugador 1 actualizamos su camara
		if(m_isPlayerOne && m_camera != nullptr){
			vector3df newRot = m_camera->GetRotation();
			vector3df rot = newRot * M_PI / 180.0;	
			SetRotation(rot);

			//Position camera FPS Y TPS
			m_camera->UpdateCamera(GetHeadPos());
		}

		// Comprobamos la velocidad maxima del jugador para que no se sobrepase
		checkMaxVelocity();

		if(m_overlayManager!=nullptr) m_overlayManager->Update(deltaTime);
	}
}

bool Player::ChangeCurrentSpell(int value){
	bool toRet = false;
	if(m_currentSpell != value){
		if(value < 0 ){
			if(m_currentSpell-1 < 0) m_currentSpell = m_numberSpells;
			else m_currentSpell--;
			toRet = true;
		}
		if(value > 3 ){
			if(m_currentSpell+1 > 3) m_currentSpell = 0;
			else m_currentSpell++;
			toRet = true;
		}
		if(value >=0 && value<= m_numberSpells){
			ResetSpell();
			m_currentSpell = value;
			toRet = true;
		}
	}
	return toRet;
}

void Player::SetSpell(int value){
	ChangeCurrentSpell(value);
}

int Player::GetCurrentSpell(){
	return m_currentSpell;
}

void Player::positionCamera(){
}

void Player::checkMaxVelocity(){
	if(m_hasCharacter){
		vector3df velocity = bt_body->GetLinearVelocity();
		
		// Fix velocity HORIZONTAL
		vector3df auxVelocityH(velocity.X,0,velocity.Z);
		float speedH = auxVelocityH.length();
		if(speedH > m_max_velocity) {
			auxVelocityH.X *= m_max_velocity/speedH;
			auxVelocityH.setY(velocity.Y);
			auxVelocityH.Z *= m_max_velocity/speedH;
			bt_body->SetLinearVelocity(auxVelocityH);
		}

		// Fix velocity VERTICAL
		vector3df auxVelocityV(0, velocity.Y, 0);
		float speedV = auxVelocityV.length();
		if(speedV > m_max_velocityY) {
			auxVelocityV.setX(auxVelocityH.X);
			auxVelocityV.Y *= m_max_velocityY/speedV;
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
	if(m_hasCharacter){
		float dTime = StateManager::GetInstance()->GetDeltaTime();
		float impulse = 500;
		impulse *= dir;
		impulse *= dTime;
		vector3df rot = m_rotation;
		bt_body->ApplyCentralImpulse(vector3df(impulse * cos(rot.Y), 0, impulse * -1 * sin(rot.Y)));
		m_moving = true;
	}
}

void Player::MoveZ(int dir){
	if(m_hasCharacter){
		float dTime = StateManager::GetInstance()->GetDeltaTime();
		float impulse = 500;
		impulse *= dir;
		impulse *= dTime;
		vector3df rot = m_rotation;
		bt_body->ApplyCentralImpulse(vector3df(impulse * sin(rot.Y), 0, impulse * cos(rot.Y)));
		m_moving = true;
	}
}

void Player::Jump() {
	if(m_CanJump && m_hasCharacter) {
		stopFootsteps();

		vector3df velocity = bt_body->GetLinearVelocity();
		velocity.Y = 0;
		bt_body->SetLinearVelocity(velocity);
		float impulse = 30 * 9.8;
		bt_body->ApplyCentralImpulse(vector3df(0,impulse,0));
		m_position.Y = bt_body->GetPosition().Y;
		
		m_CanJump = false;
		m_currentJumpCheckTime = m_maxJumpCheckTime;
	}
}

void Player::ChangeHP(float HP){

	// SERVIDOR
	if(m_networkObject != nullptr){
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
			if(m_overlayManager != nullptr) m_overlayManager->SetTime(BLOOD, 1);
			SetController(ACTION_RAYCAST, RELEASED);
		}

		// Solo le aplica danyo si su armadura es inferior a 5
		if(m_Defense<5.0f){ 
			m_HP += HP / m_Defense;
		}
		else if(m_overlayManager != nullptr)  m_overlayManager->SetTime(BLOOD, 0);
	}
	
	// AMBOS
	if(m_HP >= 100){ 
		m_HP = 100;
	}
	else if(m_HP <= 0){
		m_HP = 0;
		m_dead = true;
		if(m_overlayManager != nullptr) m_overlayManager->SetTime(BLOOD, 0);
	}
}

void Player::SetController(ACTION_ENUM action, keyStatesENUM state){
	m_controller->SetStatus(action, state);
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

	if(m_isRunning && m_moving) m_SP -= useCost;
	else m_SP += (useCost/2);
	
	if(m_SP <= 0){
		m_SP = 0;
		Run(false);
	}
	else if (m_SP > 100) m_SP = 100;
}

void Player::Respawn(){
	if(m_isPlayerOne){ 
		MenuType * current_menu = MenuManager::GetInstance()->GetCurrentMenu();
		if(current_menu != nullptr && *current_menu != ENDMATCH_M ) MenuManager::GetInstance()->ClearMenu();
		eraseTargetHUD();
	}

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();
	if(isServer && m_networkObject != nullptr){
		m_networkObject->SetBoolVar(PLAYER_RESPAWN, true, true, false);
		m_networkObject->SetBoolVar(PLAYER_RESPAWN, false, false, false);
	}

	PlayerManager::GetInstance()->AddToLife(this);
	CreatePlayerCharacter();
	SetPosition(ObjectManager::GetInstance()->GetRandomSpawnPoint(m_playerAlliance));
	PlayerInit();
}

bool Player::Raycast(){
	bool toRet = false;
	vector3df rot = GetRot();
	rot.X = -rot.X;

	vector3df Start = GetHeadPos();
	float EndX = Start.X + sin(rot.Y)*cos(rot.X)*m_raycastDistance;
	float EndY = Start.Y + sin(rot.X)*m_raycastDistance;
	float EndZ = Start.Z + cos(rot.Y)*cos(rot.X)*m_raycastDistance;

	vector3df End(EndX, EndY, EndZ);
	
	void* Object = BulletEngine::GetInstance()->Raycast(Start, End);
	if(Object!=nullptr){
		Entidad* h = (Entidad*)Object;
		h->Interact(this);
		toRet = true;
	}

	return toRet;
}

bool Player::StartSpell(){
	// Get the code of the m_currentSpell
	SPELLCODE code = SpellManager::GetInstance()->GetSpellCode(m_currentSpell, this);
	EffectManager* effectman = EffectManager::GetInstance();
	if((effectman->CheckEffect(this, WEAK_SILENCED) && code!=SPELL_PROJECTILE && code!=SPELL_CLEANSE)
	 || !SpellManager::GetInstance()->StartHechizo(m_currentSpell, this)){		// if is not a basic spell or if silenced then not shoot
		playSoundEvent(soundEvents["nomana"]);
		return false;
	}

	bool fired = SpellManager::GetInstance()->StartHechizo(m_currentSpell,this);
	return fired;
}

bool Player::ShootSpell(){
	// Get the code of the m_currentSpell
	SPELLCODE code = SpellManager::GetInstance()->GetSpellCode(m_currentSpell, this);
	EffectManager* effectman = EffectManager::GetInstance();
	
	if(effectman->CheckEffect(this, WEAK_SILENCED) && code!=SPELL_PROJECTILE && code!=SPELL_CLEANSE){		// if is not a basic spell or if silenced then not shoot
		ResetSpell();
		return false;
	}

	SetController(ACTION_RAYCAST, RELEASED);

	bool shoot = SpellManager::GetInstance()->LanzarHechizo(m_currentSpell, this);
	// En el caso de que se consiga lanzar el hechizo se enviara una senyal de sonido a la IA
	if(shoot){
		RegionalSenseManager* sense = RegionalSenseManager::GetInstance();
		sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+m_playerAlliance), 5.0f, GetKinematic(), AI_HEARING);

		// SHOOT ANIMATION
		std::vector<Hechizo*> spells = SpellManager::GetInstance()->GetSpells(this);
		std::vector<SPELLCODE> spellstypes1 = { SPELL_BASIC, SPELL_PROJECTILE, SPELL_FIRE, SPELL_POISON, SPELL_THUNDER };

		if(std::find( spellstypes1.begin(), spellstypes1.end(), spells[m_currentSpell]->GetType() ) != spellstypes1.end()){
			ChangeAnimation("shoot1", 25, false);
		}
		else{
			ChangeAnimation("shoot2", 25, false);
		}

		//m_playerNode->AddText("S: " + std::to_string(m_currentSpell), vector3df(-0.5,0.75,0), 0);
	}

	return shoot;
}

void Player::ResetAllSpells(){
	SpellManager::GetInstance()->ResetHechizo(this);
}

void Player::ResetDieSpells(){
	SpellManager::GetInstance()->ResetDieHechizo(this);
}

void Player::ResetSpell(){
	SpellManager::GetInstance()->ResetHechizo(m_currentSpell,this);
}

void Player::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();

	// id, AI_code m_name, float str, Kinematic kin, AI_modalities mod
	if(m_visible) sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+m_playerAlliance), 5.0f, GetKinematic(), AI_SIGHT);
	if(m_moving) sense->AddSignal(id, this, false, (AI_code)(AI_PLAYER_WARL+m_playerAlliance), 5.0f, GetKinematic(), AI_HEARING);
}

void Player::Die(){
	EffectManager::GetInstance()->CleanEffects(this);
	GUIEngine::GetInstance()->ShowDeathMessage(m_name,m_playerAlliance);
	ObjectManager::GetInstance()->AddPlayerParts(m_playerAlliance, m_position, m_dimensions, m_rotation);

	ResetDieSpells();										// Reseteamos los hechizos del jugador

	stopPulse();											// Stop the pulse event
	playSoundEvent(soundEvents["die"]); 												// Play the sound event
	DropObject();											// Soltamos los objetos que teniamos

	PlayerManager::GetInstance()->AddToDead(this);			// Lo anyadimos a la lista de muertos		
	if(m_matchStarted) DestroyPlayerCharacter();				// Destruimos cuerpo fisico
	if(m_overlayManager!=nullptr) GraphicEngine::getInstance()->ClearOverlay();

	EraseHUD();
}

void Player::EraseHUD(){
	if(m_hud!=nullptr)m_hud->Erase();
}

void Player::ReturnToLobby(){

	if(m_networkObject == nullptr) Respawn();

	else if(m_networkObject != nullptr){
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		bool isServer = n_engine->IsServerInit();

		if(m_isPlayerOne) m_networkObject->SetBoolVar(PLAYER_READY, false, true, false);
		else if(isServer) Respawn();
	}

}

void Player::DrawOverlays(){
	if(m_overlayManager != nullptr && m_isPlayerOne) m_overlayManager->Draw();	
}

bool Player::CheckIfReady(){
	m_readyToStart = false;

	if(m_hasCharacter){
		vector4df readyZone = ObjectManager::GetInstance()->GetReadyZone();

		bool ready = true;

		if(		m_position.X < readyZone.X
			|| 	m_position.X > readyZone.X2
			|| 	m_position.Z < readyZone.Y
			|| 	m_position.Z > readyZone.Y2){
			ready = false;	
		}

		m_readyToStart = ready;
	}

	return m_readyToStart;
}

void Player::Run(bool runStatus){
	if(m_isRunning != runStatus){
		float factor = 5/3.0f;
		
		m_isRunning = runStatus;
		if(runStatus) m_max_velocity *= factor;
		else m_max_velocity /= factor;
	}
}

void Player::CatchObject(Potion* p){
	if(m_potion == nullptr){
		ChangeAnimation("interact", 50);
		m_potion = p;
	}
}

void Player::DropObject(){
	if(m_potion!=nullptr){
		m_potion->CreatePotion(m_position, vector3df(0,0,0));
		m_potion = nullptr;
	}
}

void Player::LosePotion(){
	if(m_potion!=nullptr){
		m_potion = nullptr;
	}
	playSoundEvent(soundEvents["losepotion"]);
}

void Player::UseObject(){
	if(m_potion!=nullptr){
		// Play Drink Animation
		ChangeAnimation("drink", 50);
		
		// Play Drink Sound
		playSoundEvent(soundEvents["drink"]);

		// Use potion
		m_potion->Use(this);
		m_potion = nullptr;
	}
}

bool Player::HasObject(){
	if(m_potion!=nullptr){
		return true;
	}
	return false;
}

void Player::DeployTrap(){
	vector3df rot = GetRot();
	rot.X = -rot.X;

	vector3df Start = GetHeadPos();
	float trapRayDist = m_raycastDistance*1.5;
	float EndX = Start.X + sin(rot.Y)*cos(rot.X)*trapRayDist;
	float EndY = Start.Y + sin(rot.X)*trapRayDist;
	float EndZ = Start.Z + cos(rot.Y)*cos(rot.X)*trapRayDist;

	vector3df End(EndX, EndY, EndZ);

	void* Object = BulletEngine::GetInstance()->Raycast(Start, End);
	if(Object!=nullptr){
		Entidad* h = (Entidad*)Object;
		if(h->GetClase() == EENUM_FLOOR){
			bool putTrap = false;
			
			if(m_networkObject == nullptr) putTrap = true;
			else{
				NetworkEngine* n_engine = NetworkEngine::GetInstance();
				bool isServer = n_engine->IsServerInit();
				if(isServer) putTrap = true;
			}

			if(putTrap) TrapManager::GetInstance()->PlayerDeployTrap(this,Start,End);
		}
	}
}

void Player::ShowStatusMenu(){
	MenuManager::GetInstance()->CreateMenu(STATUS_M);

}
void Player::QuitStatusMenu(){
	MenuManager::GetInstance()->ClearMenu();
}

void Player::UpdateWalkAnimation(){
	if(m_CanJump){
		// CHANGE WALK OR IDLE ANIMATION
		int speedfps = 15;
		//speedfps = 25;
		if(m_max_velocity > 0)	
			m_walkfps = log(m_max_velocity) * 15 + speedfps;

		// CHANGE ANIMATION IDDLE
		if(m_isRunning){
			float factor = 5/3.0f;
			m_walkfps = log(m_max_velocity/factor) * 15 + speedfps;

			ChangeAnimation("run", m_walkfps, true, true);
		}
		else if(m_moving){
			ChangeAnimation("walk", m_walkfps, true, true);
		}
		else{
			ChangeAnimation("idle", m_walkfps, true, true);
		}
	}
	else{
		// IS JUMPING OR FALLING

	}
}

void Player::UpdatePosShape(float dtime){
	if(m_hasCharacter){
		m_position = bt_body->GetPosition();
		bt_body->Update();

		vector3df pos = m_position;
		pos.Y += 0.3;
		m_rotation = bt_body->GetRotation();

		// UPDATE LEGS
		m_playerNode->setPosition(pos);		// UPDATE LEGS POSITION (OR ARM)
		m_playerNode->Update(dtime);		// UPDATE ANIMATION (arm or legs)

		// UPDATE TOP PART
		if(!m_isPlayerOne && m_playerNodeTop != nullptr){
			m_playerNode->setRotation(m_rotation * 180 / M_PI);

			m_playerNodeTop->setPosition(pos);							// UPDATE POSITION
			m_playerNodeTop->setRotation(m_rotation * 180 / M_PI);		// UPDATE ROTATION
			m_playerNodeTop->Update(dtime);								// UPDATE ANIMATION (body)
		}
		else{
			m_playerNode->setRotation(m_camera->GetRotation());
		}

		// DEBUG BILLBOARDS
//		if(m_playerAlliance == ALLIANCE_WIZARD{
		/*if(!m_isPlayerOne){
			int vel = m_max_velocity;
			int decimalvel = (m_max_velocity - vel) * 10;

			vector3df pos;
			
			// SPEED
			pos = vector3df(-0.5,0.75,0);
			//m_playerNode->AddText("S: " + std::to_string(vel) + "." + std::to_string(decimalvel), pos, 0);
			//m_playerNode->AddText("T:" + std::to_string(m_playerNodeTop->GetAnimationFrame()), pos, 0);
			std::stringstream converter;
    		converter << "J:" << std::boolalpha << m_CanJump;
			m_playerNode->AddText(converter.c_str(), pos, 0);
			// FPS
			pos = vector3df(-0.5,0.5,0);
			//m_playerNode->AddText("B:" + std::to_string(m_playerNode->GetAnimationFrame()), pos, 1);

			// MOVING
			pos = vector3df(0.5,0.75,0);
			//m_playerNode->AddText("M:" + std::to_string(m_moving), pos, 2);
			//m_playerNode->AddText("D:" + std::to_string(m_playerNode->GetAnimationFrame() - m_playerNodeTop->GetAnimationFrame()), pos, 2);

			// MOVING
			pos = vector3df(0.5,0.5,0);
			//m_playerNode->AddText("F:" + std::to_string(m_walkfps), pos, 3);
		
		} // END DEBUG BILLBOARDS
*/
	}
}

bool Player::IsPlayerOne(){ return(m_isPlayerOne); }

void Player::HitMade(Player* player){
	if(m_overlayManager != nullptr) m_overlayManager->SetTime(HITLANDED, 0.205);
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
	m_stepsStarted = true;
	SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["footsteps"], GetPos());
}

void Player::playPulse() {
	if(m_isPlayerOne){
		m_pulseStarted = true;
		SoundSystem::getInstance()->checkAndPlayEvent(soundEvents["pulse"],GetPos());
	}
}

void Player::playSoundEvent(SoundEvent* event) {
	SoundSystem::getInstance()->playEvent(event, GetPos());
}

void Player::stopFootsteps() {
	m_stepsStarted = false;
	SoundSystem::getInstance()->stopEvent(soundEvents["footsteps"]);
}

void Player::stopPulse() {
	if(m_isPlayerOne) {
		m_pulseStarted = false;
		SoundSystem::getInstance()->stopEvent(soundEvents["pulse"]);
	}
}

//Update the event positions for continuous events or usable while m_moving events (like spells)
void Player::UpdateSoundsPosition(){
	if(m_stepsStarted){
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
	if(m_hasCharacter) toRet = bt_body->GetAngularVelocity();
	return toRet;
}

vector3df Player::GetPos(){ return m_position; }

float Player::GetRotY(){ return m_rotation.Y; }

vector3df Player::GetRot(){ return m_rotation; }

vector3df Player::GetCameraRot(){ 
	if(m_camera!=nullptr) return m_camera->GetRotation();
	else return vector3df(0,0,0);
}

float Player::GetWidth(){ return m_dimensions.X; }

float Player::GetHeight(){ return m_dimensions.Y; }

float Player::GetLength(){ return m_dimensions.Z; }

float Player::GetHP(){ return m_HP; }

float Player::GetMP(){ return m_MP; }

float Player::GetSP(){ return m_SP; }

float Player::GetDamageM(){ return m_DamageMult; }

NetworkObject* Player::GetNetworkObject(){ return (m_networkObject); }

Potion* Player::GetPotion(){ return m_potion; }

bool Player::GetHasCharacter(){ return m_hasCharacter; }

vector3df Player::GetVelocity(){
	vector3df toRet = vector3df(-999,-999,-999);
	if(m_hasCharacter) toRet = bt_body->GetLinearVelocity();
	return toRet;
}

Kinematic Player::GetKinematic(){
	Kinematic cKin;
	cKin.position = m_position;
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

int Player::GetNumberSpells(){ return m_numberSpells; }

bool Player::GetReadyStatus(){ return m_readyToStart; }

Alliance Player::GetAlliance(){ return m_playerAlliance; }

PlayerController* Player::GetController(){
	return m_controller;
}

std::string Player::GetName(){ return m_name; }

bool Player::GetMoving(){
	return m_moving;
}

void Player::SetAlliance(Alliance newAlliance){
	if(newAlliance == ERR_ALLIANCE) return;

	m_playerAlliance = newAlliance;

	if(m_playerAlliance == ALLIANCE_WARLOCK)
		TrapManager::GetInstance()->setPlayerUsings(this, 4);

	if(m_hasCharacter){
		DestroyPlayerGBody();
		CreatePlayerGBody();
	}

	if(m_isPlayerOne && m_networkObject != nullptr) m_networkObject->SetIntVar(PLAYER_ALLIANCE, m_playerAlliance, true, false);

	m_HP = 100;
	m_MP = 100;
	m_SP = 100;
	SetBillboard();
}

void Player::SetPosition(vector3df pos){
	if(m_hasCharacter){
		m_position = pos;
		bt_body->SetPosition(m_position);
	}
}

void Player::SetPosX(float posX){
	if(m_hasCharacter){
		m_position.X = posX;
		bt_body->SetPosition(m_position);
	}
}

void Player::SetPosY(float posY){
	if(m_hasCharacter){
		m_position.Y = posY;
		bt_body->SetPosition(m_position);
	}
}

void Player::SetRotation(vector3df rot){
	if(m_hasCharacter){
		m_rotation = rot;
		vector3df newRot = m_rotation;
		newRot.X = 0; newRot.Z = 0;
		newRot = newRot * 180 / M_PI;
		bt_body->SetRotation(newRot);
	}
}

void Player::SetHP(float HP){ m_HP = HP; }

void Player::SetSP(float SP){ m_SP = SP; }

void Player::SetDead(bool flag){ m_dead = flag; }

void Player::SetNetworkObject(NetworkObject* newNetworkObject){ m_networkObject = newNetworkObject; }

void Player::SetMatchStatus(bool started){ m_matchStarted = started; }

void Player::SetName(std::string newName){
	m_name = newName;
	if(!m_name.empty()){
		if(m_networkObject != nullptr && m_isPlayerOne) m_networkObject->SetStringVar(PLAYER_NAME, m_name, true, false);
		else if(!m_isPlayerOne) SetBillboard();
	}
}

void Player::SetVisible(bool visible){
	if(!visible) {
		m_visible = false;
		if(m_playerNodeTop != nullptr){
			m_playerNodeTop->setMaterialTexture(0, "../assets/textures/none.png");
			m_playerNodeTop->EditText("");
	}
	}
	else{
		m_visible = true;
		std::string texturePath = "./../assets/textures/Wizard.png";
		if(m_playerAlliance == ALLIANCE_WARLOCK) texturePath = "./../assets/textures/Warlock.png";

		if(m_playerNode != nullptr){
			m_playerNode->setMaterialTexture(0, texturePath.c_str());
			m_playerNode->EditText("");
		}
		if(m_playerNodeTop != nullptr){
			m_playerNode->setMaterialTexture(0, texturePath.c_str());
			m_playerNodeTop->EditText("");
		}
		
		m_playerNode->EditText(m_name);
	}
}

void Player::SetBillboard(){
	if(!m_isPlayerOne){
		m_playerNode->AddText(m_name, vector3df(0,1.25f,0), -1);
	}
}

void Player::Draw(){
	if(m_dead && m_targetDeadCam!=nullptr){
		m_targetDeadCam->Draw();
	}else{	
		/***/
		DrawOverlays();
		if(m_hud!=nullptr)m_hud->Draw();
		/***/
	}
}

bool Player::CheckIfCanJump(float deltaTime, bool forceSkip){
	if(!forceSkip) m_currentJumpCheckTime -= deltaTime;
	
	if(forceSkip || m_currentJumpCheckTime <= 0){
		m_CanJump = JumpRaycast();
		m_currentJumpCheckTime = m_maxJumpCheckTime;
	}

	return m_CanJump;
}

bool Player::JumpRaycast(){
	bool auxCanJump = false;
	float bodyLength = m_physicsDimensions.Y + 0.1f;
	float halfSize = m_physicsDimensions.X;
	float hipotenuse = sqrtf(powf(halfSize, 2) + powf(halfSize, 2));

	// Centro
	vector3df startCenter = m_position;
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

void Player::ChangeAnimation(std::string id, int fps, bool loop, bool wholeBody){
	// If is player one, wholebody doesnt matter
	if(m_isPlayerOne){
		if(loop) m_playerNode->SetAnimationLoop(id, fps);
		else m_playerNode->PlayAnimation(id, fps);
	}
	// If player is any other, has 2 parts and depends if animation is for top or whole body
	else{
		if(loop){
			if(wholeBody) m_playerNode->SetAnimationLoop(id, fps);
			m_playerNodeTop->SetAnimationLoop(id, fps);
		}
		else{
			if(wholeBody) m_playerNode->PlayAnimation(id, fps);
			m_playerNodeTop->PlayAnimation(id, fps);
		}
	}
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

void Player::SetShader(SHADERTYPE shader){
	m_playerNode->ChangeShader(shader);
}
