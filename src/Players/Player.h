#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <Actions.h>
#include "./../Entidad.h"

#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../NetworkEngine/NetworkStructs.h"
#include "./../Includes/ColliderMasks.h"
#include "./../Includes/Alliance.h"

class Potion;

class Player: public Entidad{
	public:

		Player(bool isPlayer1);
		void CreatePlayer();
		virtual void Update();
		
		void Move(float, float);
		void MoveX(int);
		void MoveZ(int);
		void Jump();
		void ChangeHP(float);
		bool ChangeMP(float);
		void Respawn();
		void Raycast();
		void Die();

		// Functions Object
		void CatchObject(Potion* p);
		void DropObject();
		void UseObject();
		void DeployTrap();

		bool IsPlayerOne();
		vector3df GetAngularVelocity();
		float GetPosX();
		float GetPosY();
		float GetPosZ();
		vector3df GetPos();
		vector3df GetHeadPos();

		float GetRotY();
		vector3df GetRot();
		float GetWidth();
		float GetHeight();
		float GetLength();
		float GetHP();
		float GetMP();
		bool GetDead();		
		float GetMaxVelocity();
		vector3df GetVelocity();
		NetworkObject* GetNetworkObject();

		void SetPosition(vector3df);
		void SetPosX(float);
		void SetPosY(float);
		void SetRotation(vector3df rotation);
		void SetHP(float);
		void SetDead(bool);
		void SetMaxVelocity(float);
		void SetNetworkObject(NetworkObject* newNetworkObject);

		virtual ~Player();

	protected:

		vector3df m_position;
		vector3df m_dimensions;
		vector3df rotation;

		float max_velocity;
		float raycastDistance;

		float m_HP;
		float m_MP;
		bool m_dead;
		bool isPlayerOne;
		Alliance playerAlliance;

		BT_Body* bt_body;
		GBody* m_playerNode;
		NetworkObject* networkObject;

		bool moving;
		bool canJump;
		float lastVerticalSpeed;

		Potion* potion;

		void checkMaxVelocity();
		void positionCamera();
		void UpdatePosShape();

};

#endif