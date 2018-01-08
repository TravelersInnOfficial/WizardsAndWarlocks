#ifndef POISONBOMB_H
#define POISONBOMB_H

#include "Projectile.h"
#include "./../PhysicsEngine/BT_GhostObject.h"

class PoisonBomb : public Projectile{

public:
    PoisonBomb(vector3df pos, vector3df dir, int emi, float damageMult = 1);
    ~PoisonBomb();

private:
    void Burst();
    // Parent functions
    void UpdatePosShape();
    void ContactBehavior();
    void ContactAction(Player* p);

    bool ready2Burst;
    bool bursted;
    int framecap;
    float lastHeight;
    float ghostScale;

    

    // Ghost object for gas explosion
    BT_GhostObject* bt_ghost;
    
};

#endif