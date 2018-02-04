#include "Projectile.h"
#include "./../Managers/BulletManager.h"
#include "./../Players/Player.h"
#include "./../Invocations/Invocation.h"
#include "./../Managers/PlayerManager.h"

Projectile::Projectile(vector3df pos,vector3df dir, int emi, float rat, float vel, int dmg, float maxDist, std::string texture, std::string soundPath){
	impact = false;
	initPos = pos;
	direction = new vector3df(dir.X, dir.Y, dir.Z);
   
	emisor = emi;

	radius = rat;
	velocity = vel;

	damage = dmg;
	maxDistance = maxDist;

	m_Texture = texture;
	clase = EENUM_PROJECTILE;
	createSoundEvent(soundPath);
	CreateProjectile();
}

Projectile::~Projectile(){
	delete direction;
	delete bt_body;
	delete m_ProjectileNode;
	if (soundEvent != NULL) soundEvent->release();
}

void Projectile::CreateProjectile(){
	// GRAPHIC ENGINE
	GraphicEngine* engine = GraphicEngine::getInstance();

	// Cargamos la esfera
	m_ProjectileNode = engine->addSphere2Scene(initPos,vector3df(0,0,0),vector3df(1,1,1), radius, -1);
	m_ProjectileNode->setScale(vector3df(1,1,1));

	// Aplicamos Material unlit y Textura
	if (m_ProjectileNode) {
		m_ProjectileNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
		m_ProjectileNode->setMaterialTexture(0, m_Texture);
	}

	// BULLET    
	bt_body = new BT_Body();
	bt_body->CreateBox(initPos, vector3df(0.7*radius,0.7*radius,0.7*radius), 50, 0,vector3df(0,0,0), C_PROJECTILE, projectileCW);
	bt_body->AssignPointer(this);

    vector3df vel(velocity * direction->X, velocity * direction->Y, velocity * direction->Z);
    bt_body->SetLinearVelocity(vel);
	
	playSoundEvent(initPos);
	
}

void Projectile::Update(){
	UpdatePosShape();

	//Comprobamos si ha llegado a la distancia maxima
	vector3df pos = bt_body->GetPosition();
	vector3df vectorDistance = pos - initPos;

	//Update the sound event position
	if (soundEvent != NULL) {
		std::cout << "update" << std::endl;
		soundEvent->setPosition(pos);
	}
	
	float currentDistance = sqrt(pow(vectorDistance.X,2)+pow(vectorDistance.Y,2)+pow(vectorDistance.Z,2));

	if(currentDistance >= maxDistance){
        BulletManager::GetInstance()->AddToDeleteProyecil(this);
	}   
}

void Projectile::UpdatePosShape(){
	bt_body->Update();
	vector3df pos = bt_body->GetPosition();
	m_ProjectileNode->setPosition(pos);
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
			if(emisor_p != NULL){emisor_p->HitMade(p);}

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
		std::cout << "no esta vacio" << std::endl;
	} else {
		std::cout << "esta vacio" << std::endl;
		soundEvent = NULL;
	}
}

void Projectile::playSoundEvent(vector3df pos) {
	if (soundEvent != NULL) {
		std::cout << "no es null" << std::endl;
		SoundSystem::getInstance()->playEvent(soundEvent, pos);
	}
	
}