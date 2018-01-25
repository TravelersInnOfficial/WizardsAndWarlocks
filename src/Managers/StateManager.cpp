#include "StateManager.h"
#include <time.h>

StateManager* StateManager::instance = 0;

StateManager::StateManager(){

	// Engines
	f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	g_engine = GraphicEngine::getInstance();
	s_engine = SoundSystem::getInstance();
	s_engine->createSystem("./../assets/banks/");
	n_engine = NetworkEngine::GetInstance();

	srand(time(0));
	currentState = NULL;
	LoadState(STATE_MENU);
	preparedStatus = WITHOUT_STATE;
}

StateManager::~StateManager(){
	std::cout<<"1"<<std::endl;
	if(currentState != NULL){
		delete currentState;	
	}
	std::cout<<"1"<<std::endl;
	delete f_engine;
	std::cout<<"1"<<std::endl;
	delete g_engine;
	std::cout<<"1"<<std::endl;
	delete s_engine;
	std::cout<<"1"<<std::endl;
	delete n_engine;
	std::cout<<"1"<<std::endl;
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
	std::cout<<"2"<<std::endl;
	if(currentState!=NULL){
		delete currentState;
	}
	std::cout<<"2"<<std::endl;
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
	std::cout<<"2"<<std::endl;
}