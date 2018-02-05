#ifndef BASICPROJECTILE_H
#define BASICPROJECTILE_H

#include "Projectile.h"
#include <EffectCodes.h>

class BasicProjectile : public Projectile{

public:
    BasicProjectile(vector3df pos, vector3df dir, int emi, EFFECTCODE effect = WEAK_BASIC, float damageMult = 1);

private:
    void ContactAction(Player* p);
    void createSoundEvent();
    EFFECTCODE contactEffect;

};

#endif