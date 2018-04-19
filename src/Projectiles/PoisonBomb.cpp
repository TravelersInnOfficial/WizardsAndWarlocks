#include "PoisonBomb.h"
#include "./../Players/Player.h"
#include "./../Managers/PlayerManager.h"
#include "./../DamageAreas/PoisonArea.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/ObjectManager.h"
#include <cmath>

PoisonBomb::PoisonBomb(vector3df pos, vector3df dir, int emi, float radius, float speed, float damageMult, float max_distance)
: Projectile(
    pos,                    // initial position of the projectile
    dir,                    // direction of the projectile
    emi,                    // player id 
    radius,                 // radius of the projectile
    speed,                  // speed of the projectile
    damageMult,             // damage of the projectile
    max_distance,           // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_POISON.png",        // texture of the projectile
    "event:/Spells/Projectiles/Eructam Troglodytam")             //Sound event
{
    //bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,-9.8,0));
    ready2Burst = false;
    
    if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(bt_body->GetPosition());
		particle->SetTexture("./../assets/textures/particles/PoisonParticle.png");
        particle->SetType(PROJECTILE_PARTICLE);
		particle->SetQuantityPerSecond(250);
	}
}

PoisonBomb::~PoisonBomb(){}

/**
 * @brief Method called when collides with a player
 * 
 * @param p 
 */
void PoisonBomb::ContactAction(Player* p){
    if(!ready2Burst){
        p->ChangeHP(-damage);
    }
}

/**
 * @brief method called after colliding with something
 * 
 */
void PoisonBomb::ContactBehavior(){
    if(!ready2Burst){
        ready2Burst = true;
    }
}

// update modified of projectile
void PoisonBomb::UpdatePosShape(){
    bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_ProjectileNode->setPosition(pos);

    float yvel = bt_body->GetLinearVelocity().Y;
    if (ready2Burst){
    
        if (fabs(yvel) < 0.000317574){
            // bomb is in the correct surface
            Player* pl = PlayerManager::GetInstance()->GetPlayerFromID(emisor);

            PoisonArea* bomb = (PoisonArea*)ObjectManager::GetInstance()->AddDamageArea(bt_body->GetPosition(), vector3df(1,1,1), vector3df(0,0,0), AREA_POISON);
            bomb->SetAlliance(pl->GetAlliance());

            BulletManager::GetInstance()->AddToDeleteProyecil(this);
        }
    }
}