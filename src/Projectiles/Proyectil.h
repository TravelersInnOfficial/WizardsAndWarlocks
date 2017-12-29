#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <vector>
#include <string>
#include <math.h>
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include <ColliderMasks.h>


class Proyectil: public Entidad{
public:
    Proyectil(vector3df pos, vector3df dir, float r, float v);
    ~Proyectil();
    void Update();
    void Contact(void* punt, EntityEnum tipo);

    BT_Body* GetBody();
    GBody* GetShape();

private:
    void CreateProyectil(vector3df pos);
	void UpdatePosShape();
	void NormalizeDir();

    int             damage;             // Danyo del proyectil

    vector3df*   	direction;	        // La direccion que va a tomar el proyectil
    float			radio;	            // Radio del proyectil
    float			velocity;           // Velocidad del proyectil

    float           maxDistance;        // Maxima distancia que puede recorrer
    vector3df       initPos;            // Posicion inicial del proyectil

    BT_Body*        bt_body;            // Cuerpo fisico del proyectil
    GBody*          m_proyectilNode;    // Cuerpo visual del proyectil
};

#endif