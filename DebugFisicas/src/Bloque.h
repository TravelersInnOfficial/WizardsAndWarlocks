#ifndef BLOQUE_H
#define BLOQUE_H

#include <btBulletDynamicsCommon.h>
#include <irrlicht/irrlicht.h>
#include <iostream>
using namespace std;

class Bloque{
public:

    void CreateBloque(btDiscreteDynamicsWorld*, irr::scene::ISceneManager*, irr::IrrlichtDevice*, irr::video::IVideoDriver*);
    void Update();
    btTransform GetPlayerTrans();
private:

    float m_posX;
	float m_posY;
	float m_posZ;

    btRigidBody* m_playerRigidBody;
	btCollisionShape* m_playerShape;
	btScalar playerMass;

    irr::scene::IAnimatedMesh* m_playerMesh;
    irr::scene::ISceneNode* m_playerNode;

};

#endif