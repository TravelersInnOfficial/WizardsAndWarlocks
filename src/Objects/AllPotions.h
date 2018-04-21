#ifndef ALLPOTIONS_H
#define ALLPOTIONS_H

#include "Potion.h"
#include <PotionTypes.h>

// ===============================================================================================//
//
//  LIFE POTION
//  Recovers 20 HP
//
// ===============================================================================================//

class LifePotion: public Potion{
public:
    LifePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

// ===============================================================================================//
//
//  MANA POTION
//  Recovers 20 MP
//
// ===============================================================================================//

class ManaPotion: public Potion{
public:
    ManaPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

// ===============================================================================================//
//
//  ICE POTION
//  Recovers 60 HP and freezes player
//
// ===============================================================================================//

class IcePotion: public Potion{
public:
    IcePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

// ===============================================================================================//
//
//  ELECTRIC POTION
//  Recovers 60 HP and paralyzes player
//
// ===============================================================================================//

class ElectricPotion: public Potion{
public:
    ElectricPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

// ===============================================================================================//
//
//  FIRE POTION
//  Recovers 20 HP and applies fire hits
//
// ===============================================================================================//

class FirePotion: public Potion{
public:
    FirePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

// ===============================================================================================//
//
//  POISON POTION
//  Recovers 20 HP and applies poison hits
//
// ===============================================================================================//

class PoisonPotion: public Potion{
public:
    PoisonPotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

// ===============================================================================================//
//
//  DEFENSE POTION
//  Recovers 20 HP and applies defense resistance
//
// ===============================================================================================//

class DefensePotion: public Potion{
public:
    DefensePotion(vector3df TPosition, vector3df TScale = vector3df(1,1,1), vector3df TRotation = vector3df(0,0,0), float value = 0.0f, std::string info = "");
    void Use(Player* p);
    bool CheckUse(Player* p);
};

#endif