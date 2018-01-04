#ifndef THUNDERPROJECTILE_H
#define THUNDERPROJECTILE_H

#include "Projectile.h"

class ThunderProjectile : public Projectile{

public:
    ThunderProjectile(vector3df pos, vector3df dir, int emi, int damageMult = 1);

private:
    void ContactAction(Player* p);

};

#endif