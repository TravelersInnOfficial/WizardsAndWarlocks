#ifndef FIREPROJECTILE_H
#define FIREPROJECTILE_H

#include "Projectile.h"

class FireProjectile : public Projectile{

public:
    FireProjectile(vector3df pos, vector3df rot, vector3df dir, int emi, float radius = 0.2, float speed = 40, float damageMult = 20, float max_distance = 20);

private:
    void CreateProjectile() override;
    void ContactAction(Player* p) override;
};

#endif