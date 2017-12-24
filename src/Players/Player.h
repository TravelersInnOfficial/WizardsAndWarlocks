#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <Actions.h>
#include <Alliance.h>
#include <ColliderMasks.h>
#include <NetworkStructs.h>
#include <kinematicTypes.h>
#include "./../Entidad.h"
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../SoundEngine/SoundSystem.h"

class Potion;

class Player: public Entidad{
	public:

		Player(bool isPlayer1);
		void CreatePlayer();
		void PlayerInit();
		virtual void Update();
		SoundEvent* dieEvent;
		SoundEvent* damageEvent;
		SoundEvent* drinkEvent;

		// Actions
		void Move(float, float);
		void MoveX(int);
		void MoveZ(int);
		void Jump();
		void ChangeHP(float);
		bool ChangeMP(float);
		void ChangeCurrentSpell(int);
		void Respawn();
		void Raycast();
		void Die();

		// Sensorial Functions
		void SendSignal();

		// Functions Object
		void CatchObject(Potion* p);
		void DropObject();
		void UseObject();
		void DeployTrap();

		// Network Functions
		void GetNetInput();
		void SetNetInput();
		NetworkObject* GetNetworkObject();

		//Geters
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
		Kinematic GetKinematic();
		vector3df GetVelocity();

		// Seters
		void SetPosition(vector3df);
		void SetPosX(float);
		void SetPosY(float);
		void SetRotation(vector3df rotation);
		void SetHP(float);
		void SetDead(bool);
		void SetMaxVelocity(float);
		void SetNetworkObject(NetworkObject* newNetworkObject);
		void SetAlliance(Alliance newAliance);

		virtual ~Player();

	protected:

		vector3df 		m_position;			// Posicion del jugador
		vector3df 		m_dimensions;		// Dimensiones del jugador
		vector3df 		rotation;			// Rotacion del jugador

		float 			max_velocity;		// Maxima Velocidad a la que puede alcanzar
		float 			raycastDistance;	// Distancia del rayo de RayCast

		int 			currentSpell;		// Hechizo Seleccionado para lanzar
		int 			numberSpells;		// Numero de hechizos del jugador [0-numberSpells]

		float 			m_HP;				// Vida del jugador - 100HP
		float			m_MP;				// Mana del jugador	- 100HP
		bool 			m_dead;				// El jugador sigue vivo? Si/No
		bool 			isPlayerOne;		// Es el jugador con el que jugamos? Si/No
		Alliance 		playerAlliance;		// Alianza del jugador [None, Wizard, Warlock]

		BT_Body* 		bt_body;			// Cuerpo físico del jugador
		GBody* 			m_playerNode;		// Cuerpo visual del jugador
		NetworkObject* 	networkObject;		// Objeto de red del jugador

		bool 			moving;				// Se esta moviendo? Si/No
		bool 			canJump;			// Puede saltar? Si/No
		float 			lastVerticalSpeed;	// Velocidad vertical en el frame anterior

		Potion* potion;						// Pocion en el inventario

		void checkMaxVelocity();			// Comprueba que no sobrepase la velocidad máxima además de alterarla
		void positionCamera();				// Actualiza la posicion de la camera
		void UpdatePosShape();				// Actualiza el cuerpo visual del jugador

};

#endif