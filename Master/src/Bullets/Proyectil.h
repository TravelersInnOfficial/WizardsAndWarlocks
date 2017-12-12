#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <vector>
#include <string>
#include <math.h>
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../Includes/ColliderMasks.h"


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

    vector3df*   	direction;	// La direccion que va a tomar el proyectil
    float			radio;		// Radio del proyectil
    float			velocity;	// Velocidad del proyectil

    float           maxDistance;
    vector3df       initPos;

    BT_Body*        bt_body;
    GBody*          m_proyectilNode;
};

#endif