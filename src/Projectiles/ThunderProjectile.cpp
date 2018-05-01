#include "ThunderProjectile.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"

ThunderProjectile::ThunderProjectile(vector3df pos, vector3df rot, vector3df dir, int emi, float radius, float speed, float damageMult, float max_distance)
: Projectile(
    pos,                        // initial position of the projectile
    rot,
    dir,                        // direction of the projectile
    emi,                        // player id 
    radius,                     // radius of the projectile
    speed,                      // speed of the projectile
    damageMult,                 // damage of the projectile
    max_distance,               // max distance the projectile can travel
    "event:/Spells/Projectiles/Furore Othinus")             //Sound event
{
    CreateProjectile();
    
    // this will be called after create projectile
    bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,0,0));
    
    if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(bt_body->GetPosition());
		particle->SetTexture("./../assets/textures/particles/ElectricParticle.png");
        particle->SetType(PROJECTILE_PARTICLE);
		particle->SetQuantityPerSecond(250);
	}
}

void ThunderProjectile::ContactAction(Player* p){
    p->ChangeHP(-damage);
    EffectManager::GetInstance()->AddEffect(p, WEAK_PARALYZED);
}

void ThunderProjectile::CreateProjectile(){
	// GRAPHIC ENGINE
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	// Cargamos la esfera
    m_ProjectileNode = g_engine->addObjMeshSceneNode("./../assets/modelos/electricball.obj");
    m_ProjectileNode->setPosition(initPos);
	m_ProjectileNode->setScale(vector3df(radius,radius,radius));
    m_ProjectileNode->setRotation(initRot);

	// BULLET    
	bt_body = new BT_Body();
	bt_body->CreateBox(initPos, vector3df(radius*0.1,radius*0.1,radius*0.1), 50, 0,vector3df(0,0,0), C_PROJECTILE, projectileCW);
	bt_body->AssignPointer(this);

    vector3df vel(velocity * direction->X, velocity * direction->Y, velocity * direction->Z);
    bt_body->SetLinearVelocity(vel);
	
	playSoundEvent(initPos);
}