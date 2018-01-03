#ifndef BASICPROJECTILE_H
#define BASICPROJECTILE_H

#include "Projectile.h"

class BasicProjectile : public Projectile{

public:
    BasicProjectile(vector3df pos, vector3df dir, int emi, int damageMult = 1);

private:
    void ContactAction(Player* p);

};

#endif