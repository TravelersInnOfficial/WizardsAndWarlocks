#include "BasicProjectile.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"

BasicProjectile::BasicProjectile(vector3df pos, vector3df dir, int emi, EFFECTCODE effect,float radius, float speed, float damageMult, float max_distance)
: Projectile(
    pos,                        // initial position of the projectile
    dir,                        // direction of the projectile
    emi,                        // player id 
    radius,                      // radius of the projectile
    speed,                      // speed of the projectile
    damageMult,            // damage of the projectile
    max_distance,                      // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_PROJECTILE.png",        // texture of the projectile
    "")
{
    // this will be called after create projectile
    bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,0,0));
    //bt_body->SetCCD(0.000004, 0.01);
    contactEffect = effect;

	if(GraphicEngine::getInstance()->GetParticleActive()){
		particle = new GParticle(bt_body->GetPosition());
		
        if(contactEffect == WEAK_SOFTBURNED) particle->SetTexture("./../assets/textures/particles/FireParticle.png");
        if(contactEffect == WEAK_SOFTPOISONED) particle->SetTexture("./../assets/textures/particles/PoisonParticle.png");
        if(contactEffect == WEAK_BASIC) particle->SetTexture("./../assets/textures/particles/NeutralParticle.png");
		
        particle->SetType(PROJECTILE_PARTICLE);
		particle->SetQuantityPerSecond(200);
	}

}

void BasicProjectile::ContactAction(Player* p){
    p->ChangeHP(-damage);
    if(contactEffect != WEAK_BASIC) EffectManager::GetInstance()->AddEffect(p, contactEffect);
}
