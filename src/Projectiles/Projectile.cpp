#include "Projectile.h"
#include "./../Managers/BulletManager.h"
#include "./../Players/Player.h"
#include "./../Objects/Invocation.h"
#include "./../Managers/PlayerManager.h"

Projectile::Projectile(vector3df pos,vector3df dir, int emi, float rat, float vel, int dmg, float maxDist, std::string texture){
	initPos = pos;
	direction = new vector3df(dir.X, dir.Y, dir.Z);
   
	emisor = emi;

	radius = rat;
	velocity = vel;

	damage = dmg;
	maxDistance = maxDist;

	m_Texture = texture;
	clase = EENUM_PROJECTILE;
	CreateProjectile();
}

Projectile::~Projectile(){
	delete direction;

	bt_body->Erase();
	m_ProjectileNode->Erase();

	delete bt_body;
	delete m_ProjectileNode;
}

void Projectile::CreateProjectile(){
	// GRAPHIC ENGINE
	GraphicEngine* engine = GraphicEngine::getInstance();

	// Cargamos el cubo
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
	bt_body->SetGravity(vector3df(0,0,0));
	bt_body->SetCollisionFlags("no_contact");
	//bt_body->SetMass(0);    
}

void Projectile::Update(){
	vector3df vel(velocity*direction->X, velocity*direction->Y, velocity*direction->Z);
	bt_body->SetLinearVelocity(vel);
	UpdatePosShape();

	//Comprobamos si ha llegado a la distancia maxima
	vector3df pos = bt_body->GetPosition();
	vector3df vectorDistance;
	vectorDistance.X = pos.X - initPos.X;
	vectorDistance.Y = pos.Y - initPos.Y;
	vectorDistance.Z = pos.Z - initPos.Z;

	float currentDistance = sqrt(pow(vectorDistance.X,2)+pow(vectorDistance.Y,2)+pow(vectorDistance.Z,2));

	if(currentDistance >= maxDistance){
		BulletManager* c = BulletManager::GetInstance();
		c->AddToDeleteProyecil(this);
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

	if(tipo==EENUM_PLAYER){
		Player* p = (Player*)punt;
		int idEmisor = p->GetId();
		if(emisor == idEmisor) return;

		Player* emisor_p = PlayerManager::GetInstance()->GetPlayerFromID(emisor);
		if(emisor_p != NULL) emisor_p->HitMade(p);
		ContactAction(p);	// Projectile hits player
	}
	else if(tipo == EENUM_INVOCATION){
		Invocation* i = (Invocation*)punt;
		i->ChangeHP(-damage);
	}
	BulletManager* c = BulletManager::GetInstance();
	c->AddToDeleteProyecil(this);
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

void Projectile::ContactAction(Player* p){
	p->ChangeHP(-damage);
}
