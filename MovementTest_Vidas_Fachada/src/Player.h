#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>
#include "./PhysicsEngine/BT_Body.h"
#include "vector3d.h"
#include <irrlicht/irrlicht.h>

class Player{
	public:

		Player();

		void CreatePlayer(irr::scene::ISceneManager*, irr::IrrlichtDevice*, irr::video::IVideoDriver*);
		void DeletePlayer( );
		void Update(irr::scene::ISceneManager*, bool);
		
		void Move(float, float);
		void MoveX(int, irr::scene::ISceneManager*);
		void MoveZ(int, irr::scene::ISceneManager*);
		void Jump();
		void ChangeHP(float);
		void Respawn(irr::scene::ISceneManager*);

		float GetPosX();
		float GetPosY();
		float GetWidth();
		float GetHeight();
		float GetLength();
		float GetHP();
		bool GetDead();
		/*btTransform GetPlayerTrans();*/

		void setPosition(float, float, float, irr::scene::ISceneManager*);
		void SetPosX(float);
		void SetPosY(float);
		void SetHP(float);
		void SetDead(bool);

		~Player();

	private:

		vector3df* m_position;
		vector3df* m_dimensions;

		float m_HP;
		bool m_dead;

		BT_Body* bt_body;

		bool moving;
		bool canJump;
		float lastVerticalSpeed;

/*
		btRigidBody* m_playerRigidBody;
		btCollisionShape* m_playerShape;
		btScalar playerMass;
*/
		irr::scene::IAnimatedMesh* m_playerMesh;
		irr::scene::ISceneNode* m_playerNode;

		void setMaxVelocity();
		void positionCamera(irr::scene::ISceneManager*);

};

#endif