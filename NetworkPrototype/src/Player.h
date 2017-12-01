#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <math.h>
#include "./Entidad.h"

#include "PhysicsEngine/BT_Body.h"
#include "GraphicEngine/GraphicEngine.h"
#include "NetworkEngine/NetworkEngine.h"

class Potion;

class Player: public Entidad{
	public:

		Player(bool isPlayer1);

		void CreatePlayer();
		void DeletePlayer();
		void Update();
		
		void Move(float, float);
		void MoveX(int);
		void MoveZ(int);
		void Jump();
		void ChangeHP(float);
		bool ChangeMP(float);
		void Respawn();
		void Raycast();

		// Functions Object
		void CatchObject(Potion* p);
		void DropObject();
		void UseObject();

		// Getters
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

		// Setters
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
		float lastVerticalSpeed;

		bool m_dead;
		bool isPlayerOne;
		bool moving;
		bool canJump;

		BT_Body* bt_body;
		GBody* m_playerNode;
		NetworkObject* networkObject;

		Potion* potion;

		void checkMaxVelocity();
		void positionCamera();

};

#endif