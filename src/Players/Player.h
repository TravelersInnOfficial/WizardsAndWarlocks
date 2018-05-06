#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <Actions.h>
#include <Alliance.h>
#include <ColliderMasks.h>
#include <EffectCodes.h>
#include <Names.h>
#include <PhysicsEngine/BT_Body.h>
#include <GraphicEngine/GraphicEngine.h>
#include <NetworkEngine/NetworkEngine.h>
#include <SoundEngine/SoundSystem.h>
#include <GUIEngine/GUIEngine.h>
#include <NetworkStructs.h>
#include <kinematicTypes.h>
#include "./PlayerController.h"
#include "./../Entidad.h"
#include "./../Cameras/Camera.h"
#include "./../Managers/OverlayManager.h"

class Potion;
class PlayerHUD;
class Trap;

class Player: public Entidad{
	public:

		Player(bool isPlayer1 = false);
		virtual ~Player();

		void InitPlayerAnimations();
		void CreatePlayerGBody();
		void CreatePlayerCharacter();

		void DestroyPlayerGBody();
		void DestroyPlayerCharacter();
		void PlayerInit();
		virtual void Update(float deltaTime);
		virtual void DeadUpdate();

		//Draw visuals
		void Draw();
		void InitHUD();
		void AddToMinimap(Player* p);
		//Erase visuals
		void EraseHUD();

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
		void UpdateSP(float deltaTime);
		void Respawn();
		bool Raycast();
		virtual void Die();
		void ReturnToLobby();
		void DrawOverlays();
		bool CheckIfReady();
		void Run(bool runningStatus);

		virtual void InitGame();			// Metodo init de Game
		virtual void RestartMatchStatus();	// Metodo init vuelta Lobby

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

		//Menu Functions
		void ShowStatusMenu();
		void QuitStatusMenu();

		// Controller
		void SetController(ACTION_ENUM action, keyStatesENUM state);

		// Network Functions
		void GetNetInput();
		void SetNetInput();
		NetworkObject* GetNetworkObject();
		void RefreshServer();				// Updatea la Red con los valores de VIDA, MANA y ALIANZA
		void HitMade(Player* player);

		//Sound Functions
		void playFootsteps();				//Plays the footsteps sound
		void playPulse();					//Plays the heart pulse sound

		void playSoundEvent(SoundEvent* event);		//Plays the parameter event

		void stopFootsteps();				//Stops the footsteps sound
		void stopPulse();					//Stops the pulse sound
		
		void UpdateSoundsPosition();		//Updates the continuous sounds
		void changeSurface(float n);		//Changes the surface parameter of the event

		//Geters
		bool IsPlayerOne();
		vector3df GetAngularVelocity();
		vector3df GetPos();
		vector3df GetHeadPos();
		float GetRotY();
		vector3df GetRot();
		vector3df GetCameraRot();
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
		bool IsDead();

		// Seters
		void SetShader(SHADERTYPE shader);
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
		void SetRandomName();

		// Public variables
		float 			m_max_velocity;		// Maxima Velocidad a la que puede alcanzar ACTUALMENTE
		float 			m_max_velocityY;
		float 			m_DamageMult;		// Multiplicador de danyo del jugador
		float 			m_Defense;			// Divisor del danyo recibido
		EFFECTCODE 		m_shotEffect;		// Efecto que aplicara al impactar la
		bool 			m_visible;			// Is player visible?
		bool 			canJump;			// Puede saltar?

	protected:

		void checkMaxVelocity();			// Comprueba que no sobrepase la velocidad máxima además de alterarla
		void positionCamera();				// Actualiza la posicion de la camera
		void UpdatePosShape(float);			// Actualiza el cuerpo visual del jugador y las animaciones
		void createSoundEvents();			//Create the sound events needed for the player
		void SetBillboard();				// Ponemos el billboard en el player
		void eraseTargetHUD();
		
		// We check if the player can jump
		float m_currentJumpCheckTime;
		float m_maxJumpCheckTime;
		bool CheckIfCanJump(float deltaTime = 0, bool forceSkip = false);
		bool JumpRaycast();

		/**
		 * @brief Changes animation of the player
		 * 
		 * @param id: id of the animation
		 * @param wholeBody: if true, applies animation to the legs
		 * @param loop: looping animation
		 */
		void ChangeAnimation(std::string id, int fps = 25, bool loop = false, bool wholeBody = false);

		vector3df 		m_position;			// Posicion del jugador
		vector3df 		m_dimensions;		// Dimensiones del jugador
		vector3df 		m_rotation;			// Rotacion del jugador

		PlayerController* m_controller;		// Objeto que controla el input del jugador

		float 			m_raycastDistance;	// Distancia del rayo de RayCast

		int 			m_currentSpell;		// Hechizo Seleccionado para lanzar
		int 			m_numberSpells;		// Numero de hechizos del jugador [0-m_numberSpells]

		float 			m_HP;				// Vida del jugador		- 100HP
		float			m_MP;				// Mana del jugador		- 100HP
		float			m_SP;				// Stamina del jugador	- 100HP

		bool 			m_dead;				// El jugador sigue vivo? Si/No
		bool 			m_isPlayerOne;		// Es el jugador con el que jugamos? Si/No
		bool			m_isRunning;			// The player is running? Yes/No

		OverlayManager* m_overlayManager;
		
		Alliance 		m_playerAlliance;		// Alianza del jugador [None, Wizard, Warlock]

		BT_Body*		bt_body;			// Cuerpo fisico del jugador
		//GBody* 		m_playerNode;		// Cuerpo visual del jugador
		GBody* 	m_playerNode;		// Cuerpo visual del jugador
		GAnimation* 	m_playerNodeTop;		// Cuerpo visual del jugador
		Camera* 		m_camera;			// Camara del player en primera o tercera persona

		NetworkObject* 	m_networkObject;		// Objeto de red del jugador
		std::string		m_name;				// Nombre del jugador en RED

		bool			m_readyToStart;		// Esta preparado para empezar la partida?
		bool			m_matchStarted;		// Ha empezado la partida?
		bool			m_hasCharacter;		// El jugador tiene un cuerpo fisico y grafico

		bool			m_stepsStarted;		// Han empezado a sonar los steps?
		bool 			m_pulseStarted;		// Pulse sound event started?
		bool 			m_moving;				// Se esta moviendo?

		Player* 		m_targetDeadCam;

		Potion* m_potion;						// Pocion en el inventario

		std::map<std::string, SoundEvent*> soundEvents;		//Sound events

		PlayerHUD* m_hud;

};

#endif