#ifndef BASICPROJECTILE_H
#define BASICPROJECTILE_H

#include "Projectile.h"
#include <EffectCodes.h>

class BasicProjectile : public Projectile{

public:
    BasicProjectile(vector3df pos, vector3df rot, vector3df dir, int emi, EFFECTCODE effect = WEAK_BASIC,float radius = 0.05, float speed = 55, float damageMult = 10, float max_distance = 20);

private:
    void CreateProjectile() override;
    void ContactAction(Player* p) override;
    void createSoundEvent();
    EFFECTCODE contactEffect;

};

#endif