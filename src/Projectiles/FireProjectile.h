#ifndef FIREPROJECTILE_H
#define FIREPROJECTILE_H

#include "Projectile.h"

class FireProjectile : public Projectile{

public:
    FireProjectile(vector3df pos, vector3df dir, int emi, int damageMult = 1);

private:
    void ContactAction(Player* p);

};

#endif