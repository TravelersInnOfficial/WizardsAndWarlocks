#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>
#include <btBulletDynamicsCommon.h>
#include <irrlicht/irrlicht.h>
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class Player{
	public:

		Player();

		void CreatePlayer(btDiscreteDynamicsWorld*, irr::scene::ISceneManager*, irr::IrrlichtDevice*, irr::video::IVideoDriver*);
		void DeletePlayer(btDiscreteDynamicsWorld* );
		void Update(irr::scene::ISceneManager*, bool);
		
		void Move(float, float);
		void MoveX(int, irr::scene::ISceneManager*);
		void MoveZ(int, irr::scene::ISceneManager*);
		void Jump();
		void ChangeHP(float);
		void Respawn();

		float GetPosX();
		float GetPosY();
		float GetWidth();
		float GetHeight();
		float GetLength();
		float GetHP();
		bool GetDead();
		btTransform GetPlayerTrans();

		void setPosition(float, float, float);
		void SetPosX(float);
		void SetPosY(float);
		void SetHP(float);
		void SetDead(bool);

		~Player();

	private:

		float m_posX;
		float m_posY;
		float m_posZ;
		float m_width;
		float m_height;
		float m_length;
		float m_HP;
		bool m_dead;

		bool moving;
		bool canJump;
		float lastVerticalSpeed;

		btRigidBody* m_playerRigidBody;
		btCollisionShape* m_playerShape;
		btScalar playerMass;

		irr::scene::IAnimatedMesh* m_playerMesh;
		irr::scene::ISceneNode* m_playerNode;

		void setMaxVelocity();
		void positionCamera(irr::scene::ISceneManager*);

};

#endif