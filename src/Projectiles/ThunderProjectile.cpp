#include "ThunderProjectile.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"

ThunderProjectile::ThunderProjectile(vector3df pos, vector3df dir, int emi, float radius, float speed, float damageMult, float max_distance)
: Projectile(
    pos,                        // initial position of the projectile
    dir,                        // direction of the projectile
    emi,                        // player id 
    radius,                     // radius of the projectile
    speed,                      // speed of the projectile
    damageMult,                 // damage of the projectile
    max_distance,               // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_THUNDER.png",        // texture of the projectile
    "event:/Spells/Projectiles/Furore Othinus")             //Sound event
{
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
