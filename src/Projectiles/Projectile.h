#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <vector>
#include <string>
#include "./../Entidad.h"

#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GraphicEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <ColliderMasks.h>
#include <ParticleData.h>

class Projectile: public Entidad{
public:
    Projectile(vector3df pos, vector3df rot, vector3df dir, int emi, float r, float v, float damage = 15, float maxDistance = 10.0f, std::string soundPath = "");
    ~Projectile();
    void Update();
    void Contact(void* punt, EntityEnum tipo);
    void playSoundEvent(vector3df pos);

    BT_Body* GetBody();
    GBody* GetShape();
    int GetDamage();

protected:
    virtual void CreateProjectile() = 0;
	void NormalizeDir();
    void createSoundEvent(std::string soundPath);
	virtual void UpdatePosShape();
    virtual void ContactBehavior();
    virtual void ContactAction(Player* p) = 0;
    

    int             emisor;             // Creador del proyectil
    bool            impact;             // Ha llegado a hacer hit? Si/No

    vector3df       initPos;            // Posicion inicial del proyectil
    vector3df*   	direction;	        // La direccion que va a tomar el proyectil
    vector3df       initRot;
    float			radius;	            // Radio del proyectil
    float			velocity;           // Velocidad del proyectil

    int             damage;             // Danyo del proyectil
    float           maxDistance;        // Maxima distancia que puede recorrer

    BT_Body*        bt_body;            // Cuerpo fisico del proyectil
    GBody*          m_ProjectileNode;    // Cuerpo visual del proyectil
    std::string     soundPath;
    SoundEvent*     soundEvent;

    GParticle* particle;
};

#endif