#ifndef ALLPOTIONS_H
#define ALLPOTIONS_H

#include "Potion.h"
#include <PotionTypes.h>

// LifePotion: Recovers Player 20 HP
class LifePotion: public Potion{
public:
    LifePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

// ManaPotion: Recovers Player 20 MP
class ManaPotion: public Potion{
public:
    ManaPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

// IcePotion: Recovers Player 60 HP and freezes player
class IcePotion: public Potion{
public:
    IcePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

// ElectricPotion: Recovers Player 60 HP and paralyzes player
class ElectricPotion: public Potion{
public:
    ElectricPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

// FirePotion: Recovers Player 20 HP and applies fire hits
class FirePotion: public Potion{
public:
    FirePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

// PoisonPotion: Recovers Player 20 HP and applies poison hits
class PoisonPotion: public Potion{
public:
    PoisonPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

// ElementalPotion: Recovers Player 20 HP and applies elemental resistance
class ElementalPotion: public Potion{
public:
    ElementalPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0));
    void Use(Player* p);
};

#endif