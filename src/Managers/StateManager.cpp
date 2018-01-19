#include "StateManager.h"

StateManager* StateManager::instance = 0;

StateManager::StateManager(){
	srand(time(0));
	currentState = NULL;
	LoadState(STATE_MENU);
	preparedStatus = WITHOUT_STATE;
}

StateManager::~StateManager(){
	if(currentState != NULL){
		delete currentState;	
	}
	instance = 0;
}

StateManager* StateManager::GetInstance(){
	if(instance==0){
		instance = new StateManager();
	}
	return instance;
}

bool StateManager::Update(){
	// En el caso de que haya un estado preparado lo cambiamos
	if(preparedStatus != WITHOUT_STATE){
		LoadState(preparedStatus);
		preparedStatus = WITHOUT_STATE;
	}

	bool end = currentState->Input();
	currentState->Update();
	currentState->Draw();

	return !end;
}

void StateManager::PrepareStatus(State_Code status){
	preparedStatus  = status;
}

void StateManager::LoadState(State_Code code){
	if(currentState!=NULL){
		delete currentState;
	}
	switch(code){
		case STATE_MENU:
			currentState = new MenuPrincipal();
			break;
		case STATE_GAME:
			currentState = new Game();
			break;
		case STATE_NETGAME:
			currentState =  NetGame::GetInstance();
			break;
		case WITHOUT_STATE:
			currentState = NULL;
			break;
	}

}