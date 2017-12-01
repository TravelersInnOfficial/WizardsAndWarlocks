#ifndef TRAP_H
#define TRAP_H

#include "vector3d.h"
#include "./PhysicsEngine/BT_GhostObject.h"
#include "./GraphicEngine/GBody.h"
#include "Entidad.h"
#include "Player.h"

enum TrapEnum{
    TENUM_DEATH_CLAWS = 0x00,
    TENUM_SPIRITS = 0x01,
    TENUM_SILENCE = 0x02,
    TENUM_TAXES = 0x03,
    TENUM_DISTURBANCE = 0x04
};

class Trap : public Entidad{
public:

    Trap();
    Trap(vector3df, TrapEnum type);

    //ENTITY METHODS
    void Contact(void*, EntityEnum);
    void Interact(Player*);

    void Deactivate();
    void Activate(Player* player);

    void SetPosition(vector3df);
    void SetDimensions(vector3df);
    void SetType(TrapEnum);

    vector3df* GetPosition();
    vector3df* GetDimensions();
    TrapEnum GetTrapType();

    void Erase();
    ~Trap();
    


private:
    TrapEnum m_trapType;

    vector3df* m_position;
    vector3df* m_dimensions;

    std::string m_texturePath;
    std::string m_effect;

    BT_GhostObject* m_body;
    BT_Body* m_rigidBody;
    GBody* g_body;

    float m_current_time;
    float m_deactivation_time;
    float m_world_time;

    void InitializeTrapData();
    void SetTrapData(vector3df, std::string, std::string);
};

#endif