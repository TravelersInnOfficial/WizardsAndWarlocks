#include "Projectile.h"
#include "./../Managers/BulletManager.h"
#include "./../Players/Player.h"
#include "./../Invocations/Invocation.h"
#include "./../Managers/PlayerManager.h"

Projectile::Projectile(vector3df pos, vector3df rot, vector3df dir, int emi, float rat, float vel, float dmg, float maxDist, std::string soundPath){
	particle = nullptr;
	
	impact = false;
	initPos = pos;
	direction = new vector3df(dir.X, dir.Y, dir.Z);
   
	emisor = emi;

	initRot = rot*180/M_PI;
	initRot.X *= -1;

	radius = rat;
	velocity = vel;

	damage = dmg;
	maxDistance = maxDist;
	
	clase = EENUM_PROJECTILE;
	createSoundEvent(soundPath);
}

Projectile::~Projectile(){
	delete direction;
	delete bt_body;
	delete m_ProjectileNode;
	if(particle != nullptr) delete particle;
	if (soundEvent != nullptr){
		soundEvent->stop();
		soundEvent->release();
		delete soundEvent;
	}
}

void Projectile::Update(){
	UpdatePosShape();

	//Comprobamos si ha llegado a la distancia maxima
	vector3df pos = bt_body->GetPosition();
	vector3df vectorDistance = pos - initPos;

	//Update the sound event position
	if (soundEvent != nullptr) soundEvent->setPosition(pos);

	float currentDistance = sqrt(pow(vectorDistance.X,2)+pow(vectorDistance.Y,2)+pow(vectorDistance.Z,2));
	if(currentDistance >= maxDistance) BulletManager::GetInstance()->AddToDeleteProyecil(this);

	if(particle != nullptr){
		particle->SetPos(bt_body->GetPosition());
		particle->Update();
	}
}

void Projectile::UpdatePosShape(){
	bt_body->Update();
	vector3df pos = bt_body->GetPosition();
	m_ProjectileNode->setPosition(pos);

	initRot.Z += 10;
	m_ProjectileNode->setRotation(initRot);
}

/**
 * @brief Enters here the moment collides with something
 * 
 * @param punt: pointer to whatever projectile contacts 
 * @param tipo: type of entity collided
 */
void Projectile::Contact(void* punt, EntityEnum tipo){
	if(!impact){		// En el caso de que no haya impactado ya
		if(tipo==EENUM_PLAYER){
			Player* p = (Player*)punt; 		//Cargamos el personaje
			int idEmisor = p->GetId();	
			if(emisor == idEmisor) return;	// En el caso de que sea quien ha lanzado el proyectil no hacemos hit

			// Cargamos el emisor del disparo y activamos su overflow de hit
			Player* emisor_p = PlayerManager::GetInstance()->GetPlayerFromID(emisor);
			if(emisor_p != nullptr){emisor_p->HitMade(p);}

			ContactAction(p);	// Projectile hits player
			impact = true;
		}
		else if(tipo == EENUM_INVOCATION){
			Invocation* i = (Invocation*)punt;
			i->ChangeHP(-damage);
			impact = true;
		}
	    ContactBehavior();
	}
}

BT_Body* Projectile::GetBody(){
	return bt_body;
 }

GBody* Projectile::GetShape(){
	return m_ProjectileNode;
}

void Projectile::NormalizeDir(){
	float length = sqrt(pow(direction->X, 2) + pow(direction->Y, 2) + pow(direction->Z,2));
	direction->X = direction->X/length;
	direction->Y = direction->Y/length;
	direction->Z = direction->Z/length;
}

/**
 * @brief This function is called when bullet contacts anything (normally will be deleted)
 * 
 */
void Projectile::ContactBehavior(){
    BulletManager::GetInstance()->AddToDeleteProyecil(this);
}

/**
 * @brief this function will be overriden by derived class
 * 
 * @param p: player collided
 */
void Projectile::ContactAction(Player* p){
	p->ChangeHP(-damage);
}

void Projectile::createSoundEvent(std::string soundPath) {
	if (soundPath != "") {
		soundEvent = SoundSystem::getInstance()->createEvent(soundPath);
	} else {
		soundEvent = nullptr;
	}
}

void Projectile::playSoundEvent(vector3df pos) {
	if (soundEvent != nullptr) {
		SoundSystem::getInstance()->playEvent(soundEvent, pos);
	}
	
}

int Projectile::GetDamage(){
	return damage;
}