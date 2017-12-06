#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>
#include <Actions.h>
#include "./Entidad.h"
#include "./ControllerPlayer.h"

#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "NetworkEngine/NetworkEngine.h"

class Potion;

class Player: public Entidad{
	public:

		Player(bool isPlayer1);

		void CreatePlayer();
		void DeletePlayer();
		void DeclareInput();
		void Update();
		void UpdateInput();
		void CheckInput();
		
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

		float GetPosX();
		float GetPosY();
		float GetPosZ();
		vector3df GetPos();
		float GetWidth();
		float GetHeight();
		float GetLength();
		float GetHP();
		float GetMP();
		bool GetDead();
		float GetRotY();
		vector3df GetRot();
		float GetMaxVelocity();
		NetworkObject* GetNetworkObject();

		void setPosition(float, float, float);
		void SetPosX(float);
		void SetPosY(float);
		void SetHP(float);
		void SetDead(bool);
		void SetMaxVelocity(float);
		void SetNetworkObject(NetworkObject* newNetworkObject);

		~Player();

	private:

		vector3df m_position;
		vector3df m_dimensions;

		float max_velocity;
		float raycastDistance;

		float m_HP;
		float m_MP;
		bool m_dead;
		bool isPlayerOne;

		BT_Body* bt_body;
		GBody* m_playerNode;
		NetworkObject* networkObject;

		bool moving;
		bool canJump;
		float lastVerticalSpeed;

		Potion* potion;
		ControllerPlayer* controller;

		void checkMaxVelocity();
		void positionCamera();

};

#endif