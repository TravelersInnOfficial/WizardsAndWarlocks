#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <vector>
#include <string>
#include <math.h> 
#include "Entidad.h"

#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"


class Proyectil: public Entidad{
public:
    Proyectil(float X, float Y, float Z, float r, float v);
    ~Proyectil();
    void CreateProyectil();
    void Update();
    void Contact(void* punt, std::string tipo);

    BT_Body* GetBody();
    GBody* GetShape();

private:
	void UpdatePosShape();
	void NormalizeDir();

    vector<float> 	direction;	// La direccion que va a tomar el proyectil
    float			radio;		// Radio del proyectil
    float			velocity;	// Velocidad del proyectil

    BT_Body*        bt_body;
    GBody*          m_proyectilNode;
};

#endif