#ifndef POISONBOMB_H
#define POISONBOMB_H

#include "Projectile.h"

#include <AreaCodes.h>

class PoisonBomb : public Projectile{

public:
    PoisonBomb(vector3df pos, vector3df rot, vector3df dir, int emi,float radius = 0.2, float speed = 15, float damageMult = 20, float max_distance = 30);
    ~PoisonBomb();

private:
    // Parent functions
    void UpdatePosShape() override;
    void ContactBehavior() override;
    void CreateProjectile() override;
    void ContactAction(Player* p) override;
    bool ready2Burst;
    
};

#endif