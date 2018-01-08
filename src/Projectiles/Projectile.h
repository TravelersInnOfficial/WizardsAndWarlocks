#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <vector>
#include <string>
#include <math.h>
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>

class Projectile: public Entidad{
public:
    Projectile(vector3df pos, vector3df dir, int emi, float r, float v, int damage = 15, float maxDistance = 10.0f, std::string texture = "./../assets/textures/wall.bmp");
    ~Projectile();
    void Update();
    void Contact(void* punt, EntityEnum tipo);

    BT_Body* GetBody();
    GBody* GetShape();

protected:
    void CreateProjectile();
	void NormalizeDir();
	virtual void UpdatePosShape();
    virtual void ContactBehavior();
    virtual void ContactAction(Player* p) = 0;

    int             emisor;             // Creador del proyectil

    vector3df       initPos;            // Posicion inicial del proyectil
    vector3df*   	direction;	        // La direccion que va a tomar el proyectil
    float			radius;	            // Radio del proyectil
    float			velocity;           // Velocidad del proyectil

    int             damage;             // Danyo del proyectil
    float           maxDistance;        // Maxima distancia que puede recorrer

    BT_Body*        bt_body;            // Cuerpo fisico del proyectil
    GBody*          m_ProjectileNode;    // Cuerpo visual del proyectil
    std::string     m_Texture;
};

#endif