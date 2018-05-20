#ifndef PLAYER_H
#define PLAYER_H

#include "./../Entidad.h"

#include <map>
#include <iostream>
#include <Actions.h>
#include <vector3d.h>
#include <Alliance.h>
#include <KeyStates.h>
#include <ShaderTypes.h>
#include <EffectCodes.h>

class Trap;
class Camera;
class Potion;
class BT_Body;
class Kinematic;
class PlayerHUD;
class GAnimation;
class SoundEvent;
class NetworkObject;
class OverlayManager;
class PlayerController;

class Player: public Entidad{
	public:

		Player(bool isPlayer1 = false);
		virtual ~Player();

		void DestroyPlayerCharacter();

		virtual void Update(float deltaTime);
		virtual void DeadUpdate(float deltatTime);

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

		// Controller
		void SetController(ACTION_ENUM action, keyStatesENUM state);
		PlayerController* GetController();

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
		
		void UpdateStepVelocity();

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
		virtual Kinematic GetKinematic();
		vector3df GetVelocity();
		Alliance GetAlliance();
		int GetNumberSpells();
		bool GetReadyStatus();
		std::string GetName();
		bool GetMoving();
		Potion* GetPotion();
		bool GetHasCharacter();
		bool IsDead();
		virtual void Debug();

		// Seters
		void SetPosition(vector3df);
		void SetPosX(float);
		void SetPosY(float);
		void SetRotation(vector3df rotation);
		void SetHP(float);
		void SetSP(float);
		void SetDead(bool);
		void SetNetworkObject(NetworkObject* newNetworkObject);
		void SetMatchStatus(bool started);
		void SetName(std::string newName);
		void SetAlliance(Alliance newAliance);
		void SetSpell(int value);
		void SetVisible(bool);
		void SetRandomName();
		void SetShader(SHADERTYPE shader);
		void SetGravity(float gravity);

		// Public variables
		float 			m_max_velocity;		// Maxima Velocidad a la que puede alcanzar ACTUALMENTE
		float 			m_max_velocityY;
		float 			m_DamageMult;		// Multiplicador de danyo del jugador
		float 			m_Defense;			// Divisor del danyo recibido
		EFFECTCODE 		m_shotEffect;		// Efecto que aplicara al impactar la
		bool 			m_visible;			// Is player visible?
		bool 			m_CanJump;			// Puede saltar?
		int 			m_walkfps;			// DEBUG: Fps a los que va actualmente la animacion de andar
		
	protected:
		void InitPlayerAnimations();
		void CreatePlayerGBody();
		void CreatePlayerCharacter();

		void DestroyPlayerGBody();
		void PlayerInit();

		void checkMaxVelocity();			// Comprueba que no sobrepase la velocidad máxima además de alterarla
		void positionCamera();				// Actualiza la posicion de la camera
		void UpdateWalkAnimation();		// Actualiza la animacion de movimiento del personaje
		void UpdatePosShape(float);			// Actualiza el cuerpo visual del jugador y las animaciones
		void createSoundEvents();			//Create the sound events needed for the player
		void SetBillboard();				// Ponemos el billboard en el player
		void eraseTargetHUD();
		
		// We check if the player can jump
		bool CheckIfCanJump(float deltaTime = 0, bool forceSkip = false);
		bool JumpRaycast();

		//Menu Functions
		void ShowStatusMenu();
		void QuitStatusMenu();

		// Dance
		void StartRandomDance();

		/**
		 * @brief Changes animation of the player
		 * 
		 * @param id: id of the animation
		 * @param wholeBody: if true, applies animation to the legs
		 * @param loop: looping animation
		 */
		void ChangeAnimation(std::string id, int fps = 25, bool loop = false, bool wholeBody = false);

		vector3df 		m_position;			// Posicion del jugador
		vector3df 		m_dimensions;		// Dimensiones del jugador visual
		vector3df 		m_physicsDimensions;// Dimensiones del jugador fisico
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
		bool 			m_moving;			// Se esta moviendo?
		bool			m_running;			// Is Player running? Yes/No
		bool 			m_dancing;			// Is Player dancing?

		OverlayManager* m_overlayManager;
		
		Alliance 		m_playerAlliance;		// Alianza del jugador [None, Wizard, Warlock]

		BT_Body*		bt_body;			// Cuerpo fisico del jugador
		GAnimation* 	m_playerNode;		// Cuerpo visual del jugador
		GAnimation* 	m_playerNodeTop;	// Cuerpo visual del jugador
		Camera* 		m_camera;			// Camara del player en primera o tercera persona

		NetworkObject* 	m_networkObject;		// Objeto de red del jugador
		std::string		m_name;				// Nombre del jugador en RED

		bool			m_readyToStart;		// Esta preparado para empezar la partida?
		bool			m_matchStarted;		// Ha empezado la partida?
		bool			m_hasCharacter;		// El jugador tiene un cuerpo fisico y grafico

		bool			m_stepsStarted;		// Han empezado a sonar los steps?
		bool 			m_pulseStarted;		// Pulse sound event started?

		Player* 		m_targetDeadCam;

		Potion* m_potion;						// Pocion en el inventario

		std::map<std::string, SoundEvent*> soundEvents;		//Sound events

		PlayerHUD* m_hud;

		float m_currentJumpCheckTime;
		float m_maxJumpCheckTime;

};

#endif