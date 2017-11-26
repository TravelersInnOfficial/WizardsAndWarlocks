#ifndef TRAP_H
#define TRAP_H

#include "vector3d.h"
#include "./PhysicsEngine/BT_GhostObject.h"
#include "./GraphicEngine/GBody.h"
#include "Entidad.h"
#include "Player.h"

enum TrapEnum: int  {
    TENUM_DEATH_CLAWS = 0,
    TENUM_SPIRITS = 1,
    TENUM_SILENCE = 2,
    TENUM_TAXES = 3,
    TENUM_DISTURBANCE = 4
};

class Trap : public Entidad{
public:

    Trap();
    Trap(vector3df, vector3df,TrapEnum type);

    //ENTITY METHODS
    void Contact(void*, EntityEnum);
    EntityEnum GetClase();

    void Deactivate();
    void Activate(Player* player);

    void SetPosition(vector3df);
    void SetDimensions(vector3df);
    void SetType(TrapEnum);

    vector3df* GetPosition();
    vector3df* GetDimensions();
    TrapEnum GetTrapType();

   // virtual void SetEffect() = 0;
    


protected:

    vector3df* m_position;
    vector3df* m_dimensions;
    //clock m_deactivation;
    bool m_deactivated;
    BT_GhostObject* m_body;
    //effect m_effect;
    std::string m_texturePath;
    TrapEnum m_trapType;
    GBody* g_body;
};

#endif