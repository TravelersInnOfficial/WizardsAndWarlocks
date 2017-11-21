#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <vector>
#include <irrlicht/irrlicht.h>
#include <string>
#include <math.h> 
#include <btBulletDynamicsCommon.h>
#include "Entidad.h"

using namespace std;

class Proyectil: public Entidad{
public:
    Proyectil(float X, float Y, float Z, float r, float v);
    ~Proyectil();
    void CreateProyectil(btDiscreteDynamicsWorld*, irr::scene::ISceneManager*, irr::IrrlichtDevice*, irr::video::IVideoDriver*);
    void Update();
    void Contact(void* punt, std::string tipo);

    btRigidBody* GetBody();
    irr::scene::ISceneNode* GetShape();

private:
	void UpdatePosShape();
	void NormalizeDir();

    vector<float> 	direction;	// La direccion que va a tomar el proyectil
    float			radio;		// Radio del proyectil
    float			velocity;	// Velocidad del proyectil

    btRigidBody* m_playerRigidBody;
	btCollisionShape* m_playerShape;
	btScalar playerMass;

    irr::scene::IAnimatedMesh* m_playerMesh;
    irr::scene::ISceneNode* m_playerNode;
};

#endif