#include "FireProjectile.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"

FireProjectile::FireProjectile(vector3df pos, vector3df dir, int emi, float damageMult)
: Projectile(
    pos,                    // initial position of the projectile
    dir,                    // direction of the projectile
    emi,                    // player id 
    0.2f,                   // radius of the projectile
    25.0f,                  // speed of the projectile
    20 * damageMult,        // damage of the projectile
    10.0f,                  // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_FIRE.png",        // texture of the projectile
    "event:/Spells/Projectiles/Dragon Breath")          //Sound event
{
    // this will be called after create projectile
    bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,0,0));
    
}

void FireProjectile::ContactAction(Player* p){
    p->ChangeHP(-damage);
    EffectManager::GetInstance()->AddEffect(p, WEAK_BURNED);
}