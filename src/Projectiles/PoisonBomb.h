#ifndef POISONBOMB_H
#define POISONBOMB_H

#include "Projectile.h"

#include <AreaCodes.h>

class PoisonBomb : public Projectile{

public:
    PoisonBomb(vector3df pos, vector3df dir, int emi, float damageMult = 1);
    ~PoisonBomb();

private:
    // Parent functions
    void UpdatePosShape();
    void ContactBehavior();
    void ContactAction(Player* p);

    bool ready2Burst;
    
};

#endif