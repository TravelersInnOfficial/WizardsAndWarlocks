#include "Trap.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include <GraphicEngine/GraphicEngine.h>
#include <PhysicsEngine/BT_GhostObject.h>
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/NetworkObject.h>
#include <NetworkEngine/Server.h>
#include "./../Managers/EffectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"
#include "./../Players/Player.h"
#include <PhysicsEngine/BT_Body.h>
#include <SoundEngine/SoundSystem.h>
#include <GraphicEngine/GParticle.h>
#include <GraphicEngine/GBody.h>
#include <GraphicEngine/GRect.h>
#include <kinematicTypes.h>
#include <ColliderMasks.h>
#include <ParticleData.h>



Trap::Trap(vector3df TPosition, vector3df normal, TrapEnum trapType){
	clase = EENUM_TRAP;
	m_normal = normal;
	m_position = vector3df(TPosition.X, TPosition.Y, TPosition.Z);
	m_rotation = new vector3df(normal.X,normal.Y,normal.Z);
	m_trapType = trapType;
	InitializeTrapData();

	m_position.Y +=0.01;

	if(m_rotation->X >= 270) m_rotation->X = m_rotation->X - 270;
	if(m_rotation->Y >= 270) m_rotation->Y = m_rotation->Y - 270;
	if(m_rotation->Z >= 270) m_rotation->Z = m_rotation->Z - 270;

	m_body = new BT_GhostObject();
	m_rigidBody = new BT_Body();

	m_deactivation_time = 3;

	m_rigidBody->CreateBox(TPosition,(*m_dimensions)*0.5,0,0);
	m_rigidBody->AssignPointer(this);
	m_rigidBody->Rotate(*m_rotation);

	g_body = GraphicEngine::getInstance()->addCube2Scene(TPosition, *m_rotation, vector3df(m_dimensions->X,m_dimensions->Y,m_dimensions->Z));
	g_body->setMaterialTexture(0,m_texturePath);

	vector3df aux_dimensions(m_dimensions->X*0.5,m_dimensions->Y*0.5+0.25,m_dimensions->Z*0.5);
	m_body->CreateGhostBox(m_position, *m_rotation, aux_dimensions, vector3df(0,aux_dimensions.Y, 0), C_TRAP, trapCW);
	m_body->AssignPointer(this);

	createSoundEvent();
	playPlaceEvent(TPosition);

	particle = nullptr;
	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(TPosition);
		particle->SetTexture("./../assets/textures/particles/TrapParticle.png");
		particle->SetType(TRAP_PARTICLE);
		particle->SetQuantityPerSecond(5);
	}

	deactivation_bar = nullptr;
	interaction_time = 0.0f;
	isInteracting = false;

	playerOne = PlayerManager::GetInstance()->GetPlayerOne();
}

void Trap::SetTrapData(vector3df dimensions, std::string texturePath, std::string effect){
	m_dimensions = new vector3df(dimensions.X, dimensions.Y, dimensions.Z);
	m_texturePath = "../assets/textures/decal.png";
	m_effect = effect;
	trapId = -1;
}

Trap::~Trap(){
	if(particle != nullptr) delete particle;
	if(deactivation_bar != nullptr) delete deactivation_bar;
	Erase();
}

void Trap::Update(float deltaTime){
	if(!isInteracting){
		if(deactivation_bar != nullptr){
			delete deactivation_bar;
			deactivation_bar = nullptr;
		}
		interaction_time = (interaction_time - deltaTime) < 0 ? 0 : (interaction_time - deltaTime);
	}
	else interaction_time += deltaTime;

	this->deltaTime = deltaTime;
	if(particle != nullptr) particle->Update();
	isInteracting = false;
}

void Trap::Update(){
	std::cout<<"NO USAR ESTE UPDATE"<<std::endl;
}

void Trap::InitializeTrapData(){
	switch(m_trapType){
		case TENUM_DEATH_CLAWS:
			SetTrapData(vector3df(0.5,0.001,0.5),"","Inmovilizacion mortal");
		break;

		case TENUM_SPIRITS:
			SetTrapData(vector3df(0.5,0.001,0.5),"","Distorsion fantasmal");
		break;

		case TENUM_SILENCE:
			SetTrapData(vector3df(0.5,0.001,0.5),"","Silenciar");
		break;

		case TENUM_TAXES:
			SetTrapData(vector3df(0.5,0.001,0.5),"","Te quita la poción que lleves encima");
		break;

		case TENUM_DISTURBANCE:
			SetTrapData(vector3df(0.5,0.001,0.5),"","Locura");
		break;

		case TENUM_EXPLOSIVE:
			SetTrapData(vector3df(0.5,0.001,0.5),"","Explosion");
		break;

		default:
			SetTrapData(vector3df(0.5,0.001,0.5),"","DEFAULT TRAP");
		break;
	}
}

void Trap::Contact(void* punt, EntityEnum tipo){
	NetworkEngine* n_engine = NetworkEngine::GetInstance();

	if(!n_engine->IsClientInit()){

		bool contacted = false;

		if(tipo == EENUM_PLAYER){
			Player* player = (Player*)(punt);
			Activate(player);
			contacted = true;
		}
		else if(tipo == EENUM_PROJECTILE){
			playExplodeEvent(m_rigidBody->GetPosition());
			TrapManager::GetInstance()->DeleteTrap(this);
			contacted = true;
		}

		if(contacted && n_engine->IsServerInit()){
			int playerAffectedId = -1;
			if(tipo == EENUM_PLAYER){
				Player* player = (Player*)(punt);
				NetworkObject* no = player->GetNetworkObject();
				if(no != nullptr) playerAffectedId = no->GetObjId();
			}

			Server* myServer = n_engine->GetServer();
			if(myServer != nullptr) myServer->EraseTrap(trapId, playerAffectedId);
		}

	}
}

void Trap::Interact(Player* p){
	isInteracting = true;
	if(p->IsPlayerOne()) deactivation_bar = new HUD_bar();
	Deactivate(deltaTime);
}

Trap::HUD_bar::HUD_bar(){
	GraphicEngine* g_engine = GraphicEngine::getInstance();
	float W = g_engine->GetScreenWidth();
	float H = g_engine->GetScreenHeight();
	float bar_height = H/40;
	bar_width = W/5;
	float posX = (W/2) - (bar_width/2);
	float posY = (H/2) - (bar_height*3);

	bkg = g_engine->add2DRect(vector2df(posX,posY), vector2df(bar_width,bar_height));
	progress_bar = g_engine->add2DRect(vector2df(posX,posY), vector2df(bar_width,bar_height));
	progress_bar->SetColor(1.0f,0.0f,0.0f);
}

Trap::HUD_bar::~HUD_bar(){
	delete bkg;
	delete progress_bar;
}

void Trap::HUD_bar::Update(float time, float total){
	float progress = bar_width * (time/total);
	progress_bar->SetWidth(progress);
}

void Trap::Activate(Player* player){

	switch(m_trapType){
		case TENUM_DEATH_CLAWS:
			EffectManager::GetInstance()->AddEffect(player, WEAK_DEATHSNARE);
			player->ChangeHP(-10);
		break;

		case TENUM_SPIRITS:
			if(player->IsPlayerOne()) EffectManager::GetInstance()->AddEffect(player, WEAK_FUZZY);
			player->ChangeHP(-25);
		break;

		case TENUM_SILENCE:
			EffectManager::GetInstance()->AddEffect(player, WEAK_SILENCED);
			player->ChangeHP(-25);
		break;

		case TENUM_TAXES:
			player->LosePotion();
			player->ChangeHP(-25);
		break;

		case TENUM_DISTURBANCE:
			EffectManager::GetInstance()->AddEffect(player, WEAK_MADNESS);
			player->ChangeHP(-25);
		break;

		case TENUM_EXPLOSIVE:
			player->ChangeHP(-35);
			playExplodeEvent(m_rigidBody->GetPosition());
		break;

		default:
		break;
	}

	if(playerOne != nullptr) playerOne->AddToMinimap(player);
	TrapManager::GetInstance()->DeleteTrap(this);
}

// FOR NET SYNC
void Trap::ForceEffect(Player* player){

	switch(m_trapType){
		case TENUM_DEATH_CLAWS:
			EffectManager::GetInstance()->AddEffect(player, WEAK_DEATHSNARE);
		break;

		case TENUM_SPIRITS:
			if(player->IsPlayerOne()) EffectManager::GetInstance()->AddEffect(player, WEAK_FUZZY);
		break;

		case TENUM_SILENCE:
			EffectManager::GetInstance()->AddEffect(player, WEAK_SILENCED);
		break;

		case TENUM_TAXES:
			player->LosePotion();
		break;

		case TENUM_DISTURBANCE:
			EffectManager::GetInstance()->AddEffect(player, WEAK_MADNESS);
		break;

		default:
		break;
	}

	TrapManager::GetInstance()->DeleteTrap(this);
}

void Trap::Deactivate(float deltaTime){
	if(deactivation_bar != nullptr) deactivation_bar->Update(interaction_time, m_deactivation_time);
	if(interaction_time >= m_deactivation_time){
		NetworkEngine* n_engine = NetworkEngine::GetInstance();
		if(!n_engine->IsClientInit()){
			TrapManager::GetInstance()->DeleteTrap(this);
			if(n_engine->IsServerInit()){
				Server* myServer = n_engine->GetServer();
				if(myServer != nullptr) myServer->EraseTrap(trapId, -1);
			}
		}
	}
}

void Trap::SetPosition(vector3df position){
	m_position.X = position.X;
	m_position.Y = position.Y;
	m_position.Z = position.Z;
}

void Trap::SetDimensions(vector3df dimensions){
	m_dimensions->X = dimensions.X;
	m_dimensions->Y = dimensions.Y;
	m_dimensions->Z = dimensions.Z;
}

void Trap::SetType(TrapEnum trapType){
	m_trapType = trapType;
}

vector3df Trap::GetPosition(){
	vector3df pos(m_position.X, m_position.Y, m_position.Z);
	return pos;
}

vector3df Trap::GetNormal(){
	return m_normal;
}

vector3df Trap::GetDimensions(){
	vector3df dim(m_dimensions->X, m_dimensions->Y, m_dimensions->Z);
	return dim;
}

TrapEnum Trap::GetTrapType(){
	return m_trapType;
}

void Trap::Erase(){
	delete m_rigidBody;
	delete m_body;
	delete g_body;

	delete m_rotation;
	delete m_dimensions;

	if (!placeEvent->isPlaying()) placeEvent->stop();
	placeEvent->release();
	delete placeEvent;


	if (!explodeEvent->isPlaying()) explodeEvent->stop();
	explodeEvent->release();
	delete explodeEvent;
}

void Trap::SendSignal(){
	RegionalSenseManager* sense = RegionalSenseManager::GetInstance();

	// id, AI_code name, float str, Kinematic kin, AI_modalities mod
	sense->AddSignal(id, this, true, AI_TRAP, 5.0f, GetKinematic(), AI_SIGHT);
}

Kinematic Trap::GetKinematic(){
	Kinematic cKin;
	cKin.position = m_rigidBody->GetPosition();
	cKin.position.Y += 0.25f;
	cKin.orientation =  vector2df(0,0);
	cKin.velocity = m_rigidBody->GetLinearVelocity();
	cKin.rotation = vector2df(0,0);
	return cKin;
}

void Trap::SetTrapId(int id){
	this->trapId = id;
}

int Trap::GetTrapId(){
	return(trapId);
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/
void Trap::createSoundEvent() {
	placeEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Shots_Spawns/Trap");
	explodeEvent = SoundSystem::getInstance()->createEvent("event:/Spells/Effects/Explosion");
}

void Trap::playExplodeEvent(vector3df pos) {
	SoundSystem::getInstance()->playEvent(explodeEvent, pos);
}

void Trap::playPlaceEvent(vector3df pos) {
	SoundSystem::getInstance()->playEvent(placeEvent, pos);
}