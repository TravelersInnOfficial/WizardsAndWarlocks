#include "PoisonBomb.h"
#include "./../Players/Player.h"
#include "./../Managers/PlayerManager.h"
#include "./../DamageAreas/PoisonArea.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/ObjectManager.h"
#include <cmath>

PoisonBomb::PoisonBomb(vector3df pos, vector3df dir, int emi, float damageMult)
: Projectile(
    pos,                    // initial position of the projectile
    dir,                    // direction of the projectile
    emi,                    // player id 
    0.2f,                   // radius of the projectile
    15.0f,                  // speed of the projectile
    20 * damageMult,        // damage of the projectile
    25.0f,                  // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_POISON.png")        // texture of the projectile
{
    //bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,-9.8,0));
    
    ready2Burst = false;
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
    //std::cout<<"YVEL: " << yvel <<"\n";
    if (ready2Burst){
    
        if (fabs(yvel) < 0.000317574){
            // bomb is in the correct surface
            Player* pl = PlayerManager::GetInstance()->GetPlayerFromID(emisor);

            PoisonArea* bomb = (PoisonArea*)ObjectManager::GetInstance()->AddDamageArea(bt_body->GetPosition(), vector3df(1,1,1), vector3df(0,0,0), AREA_POISON);
            bomb->SetAlliance(pl->GetAlliance());

            BulletManager::GetInstance()->AddToDeleteProyecil(this);
        }
        //else //std::cout<<"NO COLLIDE: LY: "<< lastHeight << " Y: " << actualHeight << " DIST: " << heightDiff << " R,B: "<< ready2Burst << ", " << bursted << std::endl;
    }
}
