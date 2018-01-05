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
#include "./PlayerController.h"
#include "./../Entidad.h"
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../SoundEngine/SoundSystem.h"

class Potion;

class Player: public Entidad{
	public:

		Player(bool isPlayer1);
		void CreatePlayerCharacter(bool firstInit = false);
		void DestroyPlayerCharacter();
		void PlayerInit();
		virtual void Update();

		// Controller
		virtual void DeclareInput();				// Metodo que declara todas las acciones del player
		void SetAllInput(keyStatesENUM state);
		void UpdateInput();
		virtual void CheckInput();

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
		virtual void Die();
		void ReturnToLobby();
		void DrawOverlays(float deltaTime);
		void CheckIfReady();

		// Spells
		bool StartSpell();
		bool ShootSpell();
		void ResetSpell();

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
		void RefreshServer();				// Updatea la Red con los valores de VIDA, MANA y ALIANZA

		//Sound Function
		void playFootsteps();				//Plays the footsteps sound
		void playDrink();					//PLays the drink potion sound
		void playDie();						//Plays the die sound
		void playHit();						//Plays the damage hit sound
		void stopFootsteps();				//Stops the footsteps sound
		void UpdateSoundsPosition();		//Updates the continuous sounds

		//Geters
		bool IsPlayerOne();
		vector3df GetAngularVelocity();
		vector3df GetPos();
		vector3df GetHeadPos();
		float GetRotY();
		vector3df GetRot();
		float GetWidth();
		float GetHeight();
		float GetLength();
		float GetHP();
		float GetMP();
		float GetDamageM();
		float GetMaxVelocity();
		Kinematic GetKinematic();
		vector3df GetVelocity();
		Alliance GetAlliance();
		int GetNumberSpells();
		bool GetReadyStatus();
		PlayerController* GetController();

		// Seters
		void SetPosition(vector3df);
		void SetPosX(float);
		void SetPosY(float);
		void SetRotation(vector3df rotation);
		void SetHP(float);
		void SetDamageMult(float);
		void SetDead(bool);
		void SetMaxVelocity(float);
		void SetNetworkObject(NetworkObject* newNetworkObject);
		void SetAlliance(Alliance newAliance);
		void SetMatchStatus(bool started);
		void SetSpell(int value);

		virtual ~Player();

	protected:

		vector3df 		m_position;			// Posicion del jugador
		vector3df 		m_dimensions;		// Dimensiones del jugador
		vector3df 		rotation;			// Rotacion del jugador

		PlayerController* controller;		// Objeto que controla el input del jugador

		float 			max_velocity;		// Maxima Velocidad a la que puede alcanzar
		float 			raycastDistance;	// Distancia del rayo de RayCast

		int 			currentSpell;		// Hechizo Seleccionado para lanzar
		int 			numberSpells;		// Numero de hechizos del jugador [0-numberSpells]

		float 			m_HP;				// Vida del jugador - 100HP
		float			m_MP;				// Mana del jugador	- 100HP
		float 			m_DamageMult;		// Multiplicador de danyo del jugador

		bool 			m_dead;				// El jugador sigue vivo? Si/No
		bool 			isPlayerOne;		// Es el jugador con el que jugamos? Si/No
		float			bloodOverlayTime;	// Tiempo de Blood Overlay que queda
		
		Alliance 		playerAlliance;		// Alianza del jugador [None, Wizard, Warlock]

		BT_Body*		bt_body;			// Cuerpo fisico del jugador
		GBody* 			m_playerNode;		// Cuerpo visual del jugador
		NetworkObject* 	networkObject;		// Objeto de red del jugador

		bool			readyToStart;		// Esta preparado para empezar la partida?
		bool			matchStarted;		// Ha empezado la partida?
		bool			hasCharacter;		// El jugador tiene un cuerpo fisico y grafico

		bool			stepsStarted;		// Han empezado a sonar los steps?

		bool 			moving;				// Se esta moviendo?
		bool 			canJump;			// Puede saltar?
		float 			lastVerticalSpeed;	// Velocidad vertical en el frame anterior

		Potion* potion;						// Pocion en el inventario

		void checkMaxVelocity();			// Comprueba que no sobrepase la velocidad máxima además de alterarla
		void positionCamera();				// Actualiza la posicion de la camera
		void UpdatePosShape();				// Actualiza el cuerpo visual del jugador
		

};

#endif