#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <btBulletDynamicsCommon.h>
#include <irrlicht/irrlicht.h>
#include "Entidad.h"
using namespace std;

class Player: public Entidad{
	public:

		Player();

		void CreatePlayer(btDiscreteDynamicsWorld*, irr::scene::ISceneManager*, irr::IrrlichtDevice*, irr::video::IVideoDriver*);
		void DeletePlayer(btDiscreteDynamicsWorld* );
		void Update();
		void Contact(void* punt, string tipo);
		void Move(float, float);
		void Jump();
		float GetPosX();
		float GetPosY();
		float GetWidth();
		float GetHeight();
		float GetLength();
		btTransform GetPlayerTrans();
		void MoveX(int);
		void MoveZ(int);
		void SetPosX(float);
		void SetPosY(float);


		~Player();

	private:

		float m_posX;
		float m_posY;
		float m_posZ;
		float m_width;
		float m_height;
		float m_length;

		btRigidBody* m_playerRigidBody;
		btCollisionShape* m_playerShape;
		btScalar playerMass;

		irr::scene::IAnimatedMesh* m_playerMesh;
		irr::scene::ISceneNode* m_playerNode;
};

#endif