#include "HumanPlayer.h"

#include "./../Managers/OverlayManager.h"
#include <NetworkEngine/NetworkEngine.h>
#include <NetworkEngine/NetworkObject.h>
#include <PhysicsEngine/BulletEngine.h>
#include <GraphicEngine/GraphicEngine.h>
#include "./../Managers/SpellManager.h"
#include "./../Objects/Potion.h"
#include "./../Cameras/Camera.h"
#include "./PlayerController.h"

HumanPlayer::HumanPlayer(bool isPlayer1):Player(isPlayer1){
	menuActivated = false;

	SpellManager* spellManager = SpellManager::GetInstance();
	spellManager->AddHechizo(0, this, SPELL_PROJECTILE);
	spellManager->AddHechizo(1, this, SPELL_FIRE);
	spellManager->AddHechizo(2, this, SPELL_DEFENSE);
	spellManager->AddHechizo(3, this, SPELL_SPEED);
	showObjInfo = 0.0f;
	m_interacting = false;
}

HumanPlayer::~HumanPlayer(){
}

void HumanPlayer::SetNetInput(){

	Player::SetNetInput();

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();

	// LO QUE SINCRONIZA EL SERVIDOR
	if(isServer){
		m_networkObject->SetVecFVar(PLAYER_POSITION, GetPos(), true, false);
		m_networkObject->SetFloatVar(PLAYER_LIFE, m_HP, true, false);
		m_networkObject->SetFloatVar(PLAYER_MANA, m_MP, true, false);
		m_networkObject->SetFloatVar(PLAYER_STAMINA, m_SP, true, false);
	}

	// LO QUE SINCRONIZA EL CLIENTE (PLAYER ONE)
	else if (m_isPlayerOne){

		// MOVEMENT
		if(m_controller->IsKeyPressed(ACTION_MOVE_LEFT)) m_networkObject->SetIntVar(PLAYER_MOVE_LEFT, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_MOVE_LEFT)) m_networkObject->SetIntVar(PLAYER_MOVE_LEFT, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_MOVE_DOWN)) m_networkObject->SetIntVar(PLAYER_MOVE_DOWN, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_MOVE_DOWN)) m_networkObject->SetIntVar(PLAYER_MOVE_DOWN, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_MOVE_RIGHT)) m_networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_MOVE_RIGHT)) m_networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_MOVE_UP)) m_networkObject->SetIntVar(PLAYER_MOVE_UP, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_MOVE_UP)) m_networkObject->SetIntVar(PLAYER_MOVE_UP, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_JUMP)) m_networkObject->SetIntVar(PLAYER_JUMP, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_JUMP)) m_networkObject->SetIntVar(PLAYER_JUMP, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_RUN)) m_networkObject->SetIntVar(PLAYER_RUN, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_RUN)) m_networkObject->SetIntVar(PLAYER_RUN, 3, true, false);

		// ACTIONS
		if(m_controller->IsKeyPressed(ACTION_RAYCAST)) m_networkObject->SetIntVar(PLAYER_RAYCAST, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_RAYCAST)) m_networkObject->SetIntVar(PLAYER_RAYCAST, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_USE_OBJECT)) m_networkObject->SetIntVar(PLAYER_USE_OBJECT, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_USE_OBJECT)) m_networkObject->SetIntVar(PLAYER_USE_OBJECT, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_DROP_OBJECT)) m_networkObject->SetIntVar(PLAYER_DROP_OBJECT, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_DROP_OBJECT)) m_networkObject->SetIntVar(PLAYER_DROP_OBJECT, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_SHOOT)) m_networkObject->SetIntVar(PLAYER_SHOOT, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_SHOOT)) m_networkObject->SetIntVar(PLAYER_SHOOT, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_DEPLOY_TRAP)) m_networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_DEPLOY_TRAP)) m_networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, 3, true, false);

		if(m_controller->IsKeyPressed(ACTION_DANCE)) m_networkObject->SetIntVar(PLAYER_DANCE, 2, true, false);
		else if(m_controller->IsKeyReleased(ACTION_DANCE)) m_networkObject->SetIntVar(PLAYER_DANCE, 3, true, false);

		// OTHERS
		if(m_hasCharacter) m_networkObject->SetVecFVar(PLAYER_ROTATION, GetRot(), true, false);

	}

}

void HumanPlayer::GetNetInput(){

	Player::GetNetInput();

	int keystate = -1;
	int objstate_int = -99999;
	vector3df objstate = vector3df(-99999,0,0);

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();
	
	// LO QUE NO RECIBE EL SERVIDOR EN NINGUN CASO, YA QUE LO SINCRONIZA EL
	if(!isServer){
		objstate = m_networkObject->GetVecFVar(PLAYER_POSITION);
		if(objstate.X != -99999){
			SetPosition(objstate);
			objstate = vector3df(-99999,0,0);
			m_networkObject->SetVecFVar(PLAYER_POSITION, objstate, false, false);
		}

		float life = -9999;
		life = m_networkObject->GetFloatVar(PLAYER_LIFE);
		if(life != -9999 && life != -1){
			if(m_HP != life){
				if(life < m_HP){
					playSoundEvent(soundEvents["hit"]);
					SetController(ACTION_RAYCAST, RELEASED);
					if(m_overlayManager!=nullptr)m_overlayManager->SetTime(BLOOD, 1);
				}
				m_HP = life;
			}
			life = -9999;
			m_networkObject->SetFloatVar(PLAYER_LIFE, life, false, false);
		}
		
		float mana = -9999;
		mana = m_networkObject->GetFloatVar(PLAYER_MANA);
		if(mana != -9999 && mana != -1){
			m_MP = mana;
			mana = -9999;
			m_networkObject->SetFloatVar(PLAYER_MANA, mana, false, false);
		}

		float stamina = -9999;
		stamina = m_networkObject->GetFloatVar(PLAYER_STAMINA);
		if(stamina != -9999 && stamina != -1){
			m_SP = stamina;
			stamina = -9999;
			m_networkObject->SetFloatVar(PLAYER_STAMINA, stamina, false, false);
		}
	}

	// LO QUE SI RECIBE TANTO EL SERVIDOR COMO TODOS LOS CLIENTES
	objstate = m_networkObject->GetVecFVar(PLAYER_ROTATION);
	if(objstate.X != -99999){
		SetRotation(objstate);
		objstate = vector3df(-99999,0,0);
	}

	// LO QUE RECIBE TANTO EL SERVIDOR COMO LOS TODOS CLIENTES, PERO NO EL PLAYER ONE DE CADA CLIENTE
	if(!m_isPlayerOne){
		objstate_int = m_networkObject->GetIntVar(PLAYER_SPELL);
		if(objstate_int != -99999){
			SetSpell(objstate_int);
			objstate_int = -99999;
			m_networkObject->SetIntVar(PLAYER_SPELL, objstate_int, false, false);
		}
		
		keystate = m_networkObject->GetIntVar(PLAYER_MOVE_LEFT);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_MOVE_LEFT, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_MOVE_LEFT, keystate, false, false);
		}
		
		keystate = m_networkObject->GetIntVar(PLAYER_MOVE_RIGHT);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_MOVE_RIGHT, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_MOVE_RIGHT, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_MOVE_UP);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_MOVE_UP, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_MOVE_UP, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_MOVE_DOWN);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_MOVE_DOWN, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_MOVE_DOWN, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_RAYCAST);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_RAYCAST, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_RAYCAST, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_JUMP);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_JUMP, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_JUMP, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_USE_OBJECT);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_USE_OBJECT, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_USE_OBJECT, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_DROP_OBJECT);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_DROP_OBJECT, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_DROP_OBJECT, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_SHOOT);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_SHOOT, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_SHOOT, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_DEPLOY_TRAP);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_DEPLOY_TRAP, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_RUN);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_RUN, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_RUN, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_DANCE);
		if(keystate != -1){
			m_controller->SetStatus(ACTION_DANCE, (keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_DANCE, keystate, false, false);
		}

		keystate = m_networkObject->GetIntVar(PLAYER_SET_ALL_INPUT);
		if(keystate != -1){
			SetAllInput((keyStatesENUM)keystate);
			keystate = -1;
			m_networkObject->SetIntVar(PLAYER_SET_ALL_INPUT, keystate, false, false);
		}
	}

}

void HumanPlayer::ToggleMenu(bool newState){
	menuActivated = newState;
	m_interacting = newState;
	if(m_camera != nullptr) m_camera->SetWorking(!menuActivated);
	GraphicEngine::getInstance()->ToggleMenu(menuActivated);
}

void HumanPlayer::CheckInput(){
	if(!menuActivated){
		// Movimiento
		if(m_controller->IsKeyDown(ACTION_MOVE_LEFT)){ MoveX(-1); }
		if(m_controller->IsKeyDown(ACTION_MOVE_DOWN)){ MoveZ(-1); }
		if(m_controller->IsKeyDown(ACTION_MOVE_RIGHT)){ MoveX(1); }
		if(m_controller->IsKeyDown(ACTION_MOVE_UP)){ MoveZ(1); }
		if(m_controller->IsKeyPressed(ACTION_JUMP)){ Jump(); }
		
		if(m_controller->IsKeyPressed(ACTION_RUN)){ Run(true); }
		if(m_controller->IsKeyReleased(ACTION_RUN)){ Run(false); }
		
		// Acciones
		if(m_controller->IsKeyDown(ACTION_RAYCAST)){ if(Raycast()) m_interacting = true;}
		if(m_controller->IsKeyReleased(ACTION_RAYCAST)){m_interacting = false;}

		if(m_controller->IsKeyPressed(ACTION_USE_OBJECT)){ UseObject();}
		if(m_controller->IsKeyPressed(ACTION_DROP_OBJECT)){ DropObject(); }
		
		// Hechizos
		if(m_controller->IsKeyPressed(ACTION_SHOOT)){ StartSpell(); }
		if(m_controller->IsKeyReleased(ACTION_SHOOT)){ ResetSpell(); }
		if(m_controller->IsKeyDown(ACTION_SHOOT)){ ShootSpell(); }

		// Cambiar hechizos
		bool spellChanged = false;
		if(m_controller->IsKeyReleased(ACTION_CHANGE_SPELL_UP)){ spellChanged = ChangeCurrentSpell(m_currentSpell +1);}
		if(m_controller->IsKeyReleased(ACTION_CHANGE_SPELL_DOWN)){ spellChanged = ChangeCurrentSpell(m_currentSpell -1); }
		if(m_controller->IsKeyPressed(ACTION_SELECT_SPELL_00)){ spellChanged = ChangeCurrentSpell(0); }
		if(m_controller->IsKeyPressed(ACTION_SELECT_SPELL_01)){ spellChanged = ChangeCurrentSpell(1); }
		if(m_controller->IsKeyPressed(ACTION_SELECT_SPELL_02)){ spellChanged = ChangeCurrentSpell(2); }
		if(m_controller->IsKeyPressed(ACTION_SELECT_SPELL_03)){ spellChanged = ChangeCurrentSpell(3); }
		if(/*spellChanged && */m_networkObject != nullptr) m_networkObject->SetIntVar(PLAYER_SPELL, m_currentSpell, true, false);
		
		// Trampas
		if(m_controller->IsKeyPressed(ACTION_DEPLOY_TRAP)){ DeployTrap(); }

		//Menus
		if(m_controller->IsKeyPressed(ACTION_SHOW_STATUS_MENU)){ ShowStatusMenu(); }
		if(m_controller->IsKeyReleased(ACTION_SHOW_STATUS_MENU)){ QuitStatusMenu(); }

		//Baile
		if(m_controller->IsKeyPressed(ACTION_DANCE)){ StartRandomDance(); }

	}
}

void HumanPlayer::Update(float deltaTime){
	Player::Update(deltaTime);
	if(!menuActivated) UpdateInput();
	if(m_isPlayerOne) LookingAtObject(); // Comprobar si estamos mirando a un item interactuable
}


void HumanPlayer::LookingAtObject(){	
		vector3df rot = GetRot();
		rot.X = -rot.X;

		vector3df Start = GetHeadPos();
		float EndX = Start.X + sin(rot.Y)*cos(rot.X)*m_raycastDistance;
		float EndY = Start.Y + sin(rot.X)*m_raycastDistance;
		float EndZ = Start.Z + cos(rot.Y)*cos(rot.X)*m_raycastDistance;

		vector3df End(EndX, EndY, EndZ);
		
		void* Object = BulletEngine::GetInstance()->Raycast(Start, End);
		if(Object!=nullptr){
			if(GraphicEngine::getInstance()->getTime() - showObjInfo > 500.0f){
				Entidad* h = (Entidad*)Object;
				EntityEnum currentEntity = h->GetClase();
				if(currentEntity != EENUM_POTION || m_potion == nullptr){
					h->ShowInteractInfo();
				}else{
					((Potion*)h)->ShowDropPotion();
				}
			}
		}
		else{
			showObjInfo = GraphicEngine::getInstance()->getTime();
		}	
}