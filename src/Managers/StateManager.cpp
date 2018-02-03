#include "StateManager.h"
#include <time.h>

StateManager* StateManager::instance = 0;

StateManager::StateManager(){
	// DeltaTime
	timeStart = 0.0f;
	deltaTime = 0.0f;

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
	resourcesLoaded = false;
}

StateManager::~StateManager(){
	if(currentState != NULL){
		delete currentState;	
	}
	delete f_engine;
	delete g_engine;
	delete s_engine;
	delete n_engine;
	instance = 0;
}

StateManager* StateManager::GetInstance(){
	if(instance==0) instance = new StateManager();
	return instance;
}

bool StateManager::Update(){
	// En el caso de que haya un estado preparado lo cambiamos
	if(preparedStatus != WITHOUT_STATE){
		LoadState(preparedStatus);
		preparedStatus = WITHOUT_STATE;
	}

	bool end = currentState->Input();
	currentState->Update(deltaTime);
	currentState->Draw();

	UpdateDelta();

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
			if(!resourcesLoaded){
				resourcesLoaded = true;
				ResourceManager::LoadResources();
			}
			currentState = new SinglePlayerGame();
			break;
		case STATE_NETGAME:
			if(!resourcesLoaded){
				resourcesLoaded = true;
				ResourceManager::LoadResources();
			}
			currentState =  new MultiPlayerGame();
			break;
		case WITHOUT_STATE:
			currentState = NULL;
			break;
	}
}

void StateManager::UpdateDelta(){
	float currentTime = g_engine->getTime() * 0.001;
	deltaTime = currentTime - timeStart;
	timeStart = currentTime;
}