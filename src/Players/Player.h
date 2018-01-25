#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <Actions.h>
#include <Alliance.h>
#include <ColliderMasks.h>
#include <EffectCodes.h>
#include <NetworkStructs.h>
#include <kinematicTypes.h>
#include "./PlayerController.h"
#include "./../Entidad.h"
#include "./../PhysicsEngine/BT_Body.h"
#include "./../GraphicEngine/GraphicEngine.h"
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../SoundEngine/SoundSystem.h"
#include "Camera.h"

class Potion;

class Player: public Entidad{
	public:

		Player(bool isPlayer1 = false);
		void CreatePlayerCharacter(bool firstInit = false);
		void DestroyPlayerCharacter();
		void PlayerInit();
		virtual void Update();
		virtual void DeadUpdate();

		// Draw
		void Draw();
		void DrawBars();
		void DrawSpellSelector();
		void DrawInventory();
		void DrawTraps();

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
		void UpdateSP();
		void Respawn();
		void Raycast();
		virtual void Die();
		void ReturnToLobby();
		void DrawOverlays(float deltaTime);
		void CheckIfReady();
		void Run(bool runningStatus);
		void ApplyFuzyEffect();

		// Spells
		bool StartSpell();
		bool ShootSpell();
		void ResetSpell();
		bool ChangeCurrentSpell(int);
		int GetCurrentSpell();
		void ResetAllSpells();
		void ResetDieSpells();

		// Sensorial Functions
		void SendSignal();

		// Functions Object
		void CatchObject(Potion* p);
		void DropObject();
		void LosePotion();
		void UseObject();
		void DeployTrap();
		bool HasObject();

		// Network Functions
		void GetNetInput();
		void SetNetInput();
		NetworkObject* GetNetworkObject();
		void RefreshServer();				// Updatea la Red con los valores de VIDA, MANA y ALIANZA
		void HitMade(Player* player);

		//Sound Function
		void playFootsteps();				//Plays the footsteps sound
		void playDrink();					//PLays the drink potion sound
		void playDie();						//Plays the die sound
		void playHit();						//Plays the damage hit sound
		void playPulse();					//Plays the heart pulse sound
		void stopFootsteps();				//Stops the footsteps sound
		void stopPulse();
		void UpdateSoundsPosition();		//Updates the continuous sounds
		void changeSurface(float n);		//Changes the surface parameter of the event

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
		float GetSP();
		float GetDamageM();
		Kinematic GetKinematic();
		vector3df GetVelocity();
		Alliance GetAlliance();
		int GetNumberSpells();
		bool GetReadyStatus();
		PlayerController* GetController();
		std::string GetName();
		bool GetMoving();
		Potion* GetPotion();
		bool GetHasCharacter();

		// Seters
		void SetPosition(vector3df);
		void SetPosX(float);
		void SetPosY(float);
		void SetRotation(vector3df rotation);
		void SetHP(float);
		void SetSP(float);
		void SetDamageMult(float);
		void SetDead(bool);
		void SetNetworkObject(NetworkObject* newNetworkObject);
		void SetAlliance(Alliance newAliance);
		void SetMatchStatus(bool started);
		void SetSpell(int value);
		void SetName(std::string newName);
		void SetVisible(bool);

		virtual ~Player();

		// public variables
		float 			max_velocity;		// Maxima Velocidad a la que puede alcanzar ACTUALMENTE
		float 			m_DamageMult;		// Multiplicador de danyo del jugador
		float 			m_Defense;			// Divisor del danyo recibido
		EFFECTCODE 		m_shotEffect;		// Efecto que aplicara al impactar la
		bool 			m_visible;			// Is player visible?
		bool 			canJump;			// Puede saltar?
		bool			m_Able2Jump;			// Is able to recover jump?

	protected:

		void checkMaxVelocity();			// Comprueba que no sobrepase la velocidad máxima además de alterarla
		void positionCamera();				// Actualiza la posicion de la camera
		void UpdatePosShape();				// Actualiza el cuerpo visual del jugador
		void createSoundEvents();			//Create the sound events needed for the player
		void SetBillboard();				// Ponemos el billboard en el player

		vector3df 		m_position;			// Posicion del jugador
		vector3df 		m_dimensions;		// Dimensiones del jugador
		vector3df 		rotation;			// Rotacion del jugador

		PlayerController* controller;		// Objeto que controla el input del jugador

		float 			raycastDistance;	// Distancia del rayo de RayCast

		int 			currentSpell;		// Hechizo Seleccionado para lanzar
		int 			numberSpells;		// Numero de hechizos del jugador [0-numberSpells]

		float 			m_HP;				// Vida del jugador		- 100HP
		float			m_MP;				// Mana del jugador		- 100HP
		float			m_SP;				// Stamina del jugador	- 100HP

		bool 			m_dead;				// El jugador sigue vivo? Si/No
		bool 			isPlayerOne;		// Es el jugador con el que jugamos? Si/No
		bool			isRunning;			// The player is running? Yes/No

		float			bloodOverlayTime;	//|
		float			hitOverlayTime;		//|
		float			fuzzyOverlayTime;	//| Tiempo de Blood Overlay que queda
		
		Alliance 		playerAlliance;		// Alianza del jugador [None, Wizard, Warlock]

		BT_Body*		bt_body;			// Cuerpo fisico del jugador
		GBody* 			m_playerNode;		// Cuerpo visual del jugador
		Camera* 		m_camera;			// Camara del player en primera o tercera persona

		NetworkObject* 	networkObject;		// Objeto de red del jugador
		std::string		name;				// Nombre del jugador en RED

		bool			readyToStart;		// Esta preparado para empezar la partida?
		bool			matchStarted;		// Ha empezado la partida?
		bool			hasCharacter;		// El jugador tiene un cuerpo fisico y grafico

		bool			stepsStarted;		// Han empezado a sonar los steps?
		bool 			pulseStarted;		// Pulse sound event started?
		bool 			moving;				// Se esta moviendo?
		float 			lastVerticalSpeed;	// Velocidad vertical en el frame anterior

		Player* 		targetDeadCam;

		Potion* potion;						// Pocion en el inventario

		std::map<std::string, SoundEvent*> soundEvents;		//Sound events
};

#endif