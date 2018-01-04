#include "ThunderProjectile.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"

ThunderProjectile::ThunderProjectile(vector3df pos, vector3df dir, int emi, int damageMult)
: Projectile(
    pos,            // initial position of the projectile
    dir,            // direction of the projectile
    emi,            // player id 
    0.15f,          // radius of the projectile
    30.0f,          // speed of the projectile
    20,             // damage of the projectile
    10.0f,          // max distance the projectile can travel
    "./../assets/textures/projectils/thunderball.png")        // texture of the projectile
{         

}

void ThunderProjectile::ContactAction(Player* p){
    p->ChangeHP(-damage);
    EffectManager::GetInstance()->AddEffect(p, WEAK_PARALYZED);
}
