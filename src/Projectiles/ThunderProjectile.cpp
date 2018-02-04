#include "ThunderProjectile.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"

ThunderProjectile::ThunderProjectile(vector3df pos, vector3df dir, int emi, float damageMult)
: Projectile(
    pos,                        // initial position of the projectile
    dir,                        // direction of the projectile
    emi,                        // player id 
    0.15f,                      // radius of the projectile
    30.0f,                      // speed of the projectile
    20 * damageMult,            // damage of the projectile
    10.0f,                      // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_THUNDER.png",        // texture of the projectile
    "event:/Spells/Projectiles/Furore Othinus")             //Sound event
{
    // this will be called after create projectile
    bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,0,0));
}

void ThunderProjectile::ContactAction(Player* p){
    p->ChangeHP(-damage);
    EffectManager::GetInstance()->AddEffect(p, WEAK_PARALYZED);
}
