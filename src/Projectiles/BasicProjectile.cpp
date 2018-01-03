#include "BasicProjectile.h"
#include "../Players/Player.h"

BasicProjectile::BasicProjectile(vector3df pos, vector3df dir, int emi, int damageMult)
: Projectile(
    pos,            // initial position of the projectile
    dir,            // direction of the projectile
    emi,            // player id 
    0.05f,          // radius of the projectile
    30.0f,          // speed of the projectile
    15,             // damage of the projectile
    10.0f,          // max distance the projectile can travel
    "./../assets/textures/projectils/basicball.png")        // texture of the projectile
{         

}

void BasicProjectile::ContactAction(Player* p){
    p->ChangeHP(-damage);
}
