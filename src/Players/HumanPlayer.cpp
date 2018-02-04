#include "HumanPlayer.h"
#include "./../Managers/SpellManager.h"

HumanPlayer::HumanPlayer(bool isPlayer1):Player(isPlayer1){
	menuActivated = false;
}

HumanPlayer::~HumanPlayer(){
}

void HumanPlayer::SetNetInput(){

	Player::SetNetInput();

	NetworkEngine* n_engine = NetworkEngine::GetInstance();
	bool isServer = n_engine->IsServerInit();

	// LO QUE SINCRONIZA EL SERVIDOR
	if(isServer){
		networkObject->SetVecFVar(PLAYER_POSITION, GetPos(), true, false);
		networkObject->SetFloatVar(PLAYER_LIFE, m_HP, true, false);
		networkObject->SetFloatVar(PLAYER_MANA, m_MP, true, false);
		networkObject->SetFloatVar(PLAYER_STAMINA, m_SP, true, false);
	}

	// LO QUE SINCRONIZA EL CLIENTE (PLAYER ONE)
	else if (isPlayerOne){

		// MOVEMENT
		if(controller->IsKeyPressed(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_MOVE_LEFT)) networkObject->SetIntVar(PLAYER_MOVE_LEFT, 3, true, false);

		if(controller->IsKeyPressed(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_MOVE_DOWN)) networkObject->SetIntVar(PLAYER_MOVE_DOWN, 3, true, false);

		if(controller->IsKeyPressed(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_MOVE_RIGHT)) networkObject->SetIntVar(PLAYER_MOVE_RIGHT, 3, true, false);

		if(controller->IsKeyPressed(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_MOVE_UP)) networkObject->SetIntVar(PLAYER_MOVE_UP, 3, true, false);

		if(controller->IsKeyPressed(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_JUMP)) networkObject->SetIntVar(PLAYER_JUMP, 3, true, false);

		if(controller->IsKeyPressed(ACTION_RUN)) networkObject->SetIntVar(PLAYER_RUN, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_RUN)) networkObject->SetIntVar(PLAYER_RUN, 3, true, false);

		// ACTIONS
		if(controller->IsKeyPressed(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_RAYCAST)) networkObject->SetIntVar(PLAYER_RAYCAST, 3, true, false);

		if(controller->IsKeyPressed(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_USE_OBJECT)) networkObject->SetIntVar(PLAYER_USE_OBJECT, 3, true, false);

		if(controller->IsKeyPressed(ACTION_DROP_OBJECT)) networkObject->SetIntVar(PLAYER_DROP_OBJECT, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_DROP_OBJECT)) networkObject->SetIntVar(PLAYER_DROP_OBJECT, 3, true, false);

		if(controller->IsKeyPressed(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_SHOOT)) networkObject->SetIntVar(PLAYER_SHOOT, 3, true, false);

		if(controller->IsKeyPressed(ACTION_DEPLOY_TRAP)) networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, 2, true, false);
		else if(controller->IsKeyReleased(ACTION_DEPLOY_TRAP)) networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, 3, true, false);

		// OTHERS
		if(hasCharacter) networkObject->SetVecFVar(PLAYER_ROTATION, GetRot(), true, false);

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
		objstate = networkObject->GetVecFVar(PLAYER_POSITION);
		if(objstate.X != -99999){
			SetPosition(objstate);
			objstate = vector3df(-99999,0,0);
			networkObject->SetVecFVar(PLAYER_POSITION, objstate, false, false);
		}

		float life = -9999;
		life = networkObject->GetFloatVar(PLAYER_LIFE);
		if(life != -9999 && life != -1){
			if(m_HP != life){
				if(life < m_HP){
					playHit();
					SetController(ACTION_RAYCAST, RELEASED);
					if(overlayManager!=NULL)overlayManager->SetTime(BLOOD, 1);
				}
				m_HP = life;
			}
			life = -9999;
			networkObject->SetFloatVar(PLAYER_LIFE, life, false, false);
		}
		
		float mana = -9999;
		mana = networkObject->GetFloatVar(PLAYER_MANA);
		if(mana != -9999 && mana != -1){
			m_MP = mana;
			mana = -9999;
			networkObject->SetFloatVar(PLAYER_MANA, mana, false, false);
		}

		float stamina = -9999;
		stamina = networkObject->GetFloatVar(PLAYER_STAMINA);
		if(stamina != -9999 && stamina != -1){
			m_SP = stamina;
			stamina = -9999;
			networkObject->SetFloatVar(PLAYER_STAMINA, stamina, false, false);
		}
	}

	// LO QUE SI RECIBE TANTO EL SERVIDOR COMO TODOS LOS CLIENTES
	objstate = networkObject->GetVecFVar(PLAYER_ROTATION);
	if(objstate.X != -99999){
		SetRotation(objstate);
		objstate = vector3df(-99999,0,0);
	}

	// LO QUE RECIBE TANTO EL SERVIDOR COMO LOS TODOS CLIENTES, PERO NO EL PLAYER ONE DE CADA CLIENTE
	if(!isPlayerOne){
		objstate_int = networkObject->GetIntVar(PLAYER_SPELL);
		if(objstate_int != -99999){
			SetSpell(objstate_int);
			objstate_int = -99999;
			networkObject->SetIntVar(PLAYER_SPELL, objstate_int, false, false);
		}
		
		keystate = networkObject->GetIntVar(PLAYER_MOVE_LEFT);
		if(keystate != -1){
			controller->SetStatus(ACTION_MOVE_LEFT, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_MOVE_LEFT, keystate, false, false);
		}
		
		keystate = networkObject->GetIntVar(PLAYER_MOVE_RIGHT);
		if(keystate != -1){
			controller->SetStatus(ACTION_MOVE_RIGHT, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_MOVE_RIGHT, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_MOVE_UP);
		if(keystate != -1){
			controller->SetStatus(ACTION_MOVE_UP, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_MOVE_UP, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_MOVE_DOWN);
		if(keystate != -1){
			controller->SetStatus(ACTION_MOVE_DOWN, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_MOVE_DOWN, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_RAYCAST);
		if(keystate != -1){
			controller->SetStatus(ACTION_RAYCAST, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_RAYCAST, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_JUMP);
		if(keystate != -1){
			controller->SetStatus(ACTION_JUMP, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_JUMP, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_USE_OBJECT);
		if(keystate != -1){
			controller->SetStatus(ACTION_USE_OBJECT, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_USE_OBJECT, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_DROP_OBJECT);
		if(keystate != -1){
			controller->SetStatus(ACTION_DROP_OBJECT, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_DROP_OBJECT, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_SHOOT);
		if(keystate != -1){
			controller->SetStatus(ACTION_SHOOT, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_SHOOT, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_DEPLOY_TRAP);
		if(keystate != -1){
			controller->SetStatus(ACTION_DEPLOY_TRAP, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_DEPLOY_TRAP, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_RUN);
		if(keystate != -1){
			controller->SetStatus(ACTION_RUN, (keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_RUN, keystate, false, false);
		}

		keystate = networkObject->GetIntVar(PLAYER_SET_ALL_INPUT);
		if(keystate != -1){
			SetAllInput((keyStatesENUM)keystate);
			keystate = -1;
			networkObject->SetIntVar(PLAYER_SET_ALL_INPUT, keystate, false, false);
		}
	}

}

void HumanPlayer::ToggleMenu(bool newState){
	menuActivated = newState;
	GraphicEngine::getInstance()->ToggleMenu(menuActivated);
}

void HumanPlayer::CheckInput(){
	if(!menuActivated){
		// Movimiento
		if(controller->IsKeyDown(ACTION_MOVE_LEFT)){ this->MoveX(-1); }
		if(controller->IsKeyDown(ACTION_MOVE_DOWN)){ this->MoveZ(-1); }
		if(controller->IsKeyDown(ACTION_MOVE_RIGHT)){ this->MoveX(1); }
		if(controller->IsKeyDown(ACTION_MOVE_UP)){ this->MoveZ(1); }
		if(controller->IsKeyPressed(ACTION_JUMP)){ this->Jump(); }
		
		if(controller->IsKeyPressed(ACTION_RUN)){ this->Run(true); }
		if(controller->IsKeyReleased(ACTION_RUN)){ this->Run(false); }
		
		// Acciones
		if(controller->IsKeyDown(ACTION_RAYCAST)){ this->Raycast(); }

		if(controller->IsKeyPressed(ACTION_USE_OBJECT)){ this->UseObject();}
		if(controller->IsKeyPressed(ACTION_DROP_OBJECT)){ this->DropObject(); }
		
		// Hechizos
		if(controller->IsKeyPressed(ACTION_SHOOT)){ StartSpell(); }
		if(controller->IsKeyReleased(ACTION_SHOOT)){ ResetSpell(); }
		if(controller->IsKeyDown(ACTION_SHOOT)){ ShootSpell(); }

		// Cambiar hechizos
		bool spellChanged = false;
		if(controller->IsKeyReleased(ACTION_CHANGE_SPELL_UP)){ spellChanged = ChangeCurrentSpell(currentSpell +1);}
		if(controller->IsKeyReleased(ACTION_CHANGE_SPELL_DOWN)){ spellChanged = ChangeCurrentSpell(currentSpell -1); }
		if(controller->IsKeyPressed(ACTION_SELECT_SPELL_00)){ spellChanged = ChangeCurrentSpell(0); }
		if(controller->IsKeyPressed(ACTION_SELECT_SPELL_01)){ spellChanged = ChangeCurrentSpell(1); }
		if(controller->IsKeyPressed(ACTION_SELECT_SPELL_02)){ spellChanged = ChangeCurrentSpell(2); }
		if(controller->IsKeyPressed(ACTION_SELECT_SPELL_03)){ spellChanged = ChangeCurrentSpell(3); }
		if(spellChanged && networkObject != NULL) networkObject->SetIntVar(PLAYER_SPELL, currentSpell, true, false);
		
		// Trampas
		if(controller->IsKeyPressed(ACTION_DEPLOY_TRAP)){ this->DeployTrap(); }
	}
}

void HumanPlayer::Update(float deltaTime){
	Player::Update(deltaTime);
	if(!menuActivated) UpdateInput();
}