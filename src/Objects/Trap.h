#ifndef TRAP_H
#define TRAP_H

#include <vector3d.h>
#include <TrapCodes.h>
#include <PhysicsEngine/BT_GhostObject.h>
#include <GraphicEngine/GBody.h>
#include <SoundEngine/SoundSystem.h>
#include "./../Players/Player.h"
#include "./../Entidad.h"
#include <GraphicEngine/GParticle.h>

class Trap : public Entidad{
public:

    Trap(vector3df, vector3df normal, TrapEnum type);

    void Update();
    void Update(float deltaTime);

    //ENTITY METHODS
    void Contact(void*, EntityEnum);
    void Interact(Player*);

    void Deactivate(float deltaTime);
    void Activate(Player* player);

    void SetPosition(vector3df);
    void SetDimensions(vector3df);
    void SetType(TrapEnum);
    void SetTrapId(int id);

    vector3df GetPosition();
    vector3df GetDimensions();
    TrapEnum GetTrapType();
    vector3df GetNormal();
    int GetTrapId();

    // ONLY FOR NET SYNC
    void ForceEffect(Player* player);

    // Sense Functions
    void SendSignal();
    Kinematic GetKinematic();

    void Erase();
    ~Trap();

    //Sound Functions
    void playPlaceEvent(vector3df pos);
    void playExplodeEvent(vector3df pos);

protected:
    SoundEvent* placeEvent;
    SoundEvent* explodeEvent;
    void createSoundEvent();

private:
    TrapEnum m_trapType;

    vector3df m_normal;
    vector3df* m_position;
    vector3df* m_dimensions;
    vector3df* m_rotation;

    std::string m_texturePath;
    std::string m_effect;

    BT_GhostObject* m_body;
    BT_Body* m_rigidBody;
    GBody* g_body;

    struct HUD_bar{
        HUD_bar();
        ~HUD_bar();
        void Update(float time, float total);
        float bar_width;
        GRect* bkg;
        GRect* progress_bar;
    };

    float deltaTime;
    float m_deactivation_time;
    float interaction_time;
    bool isInteracting;

    int trapId;

    GParticle* particle;
    HUD_bar* deactivation_bar;

    void InitializeTrapData();
    void SetTrapData(vector3df, std::string, std::string);
};

#endif