#include "StateManager.h"
#include <time.h>

StateManager* StateManager::instance = 0;

StateManager::StateManager(bool isServer){
	// DeltaTime
	timeStart = 0.0f;
	deltaTime = 0.0f;

	// Engines
	g_engine = GraphicEngine::getInstance(isServer);
	f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	s_engine = SoundSystem::getInstance();
	s_engine->createSystem("./../assets/banks/");
	n_engine = NetworkEngine::GetInstance();

	srand(time(0));
	currentState = NULL;
	
	State_Code firstState = STATE_MENU;
	if(isServer) firstState = STATE_NETGAME_SERVER;
	
	LoadState(firstState);
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

StateManager* StateManager::GetInstance(bool isServer){
	if(instance==0) instance = new StateManager(isServer);
	return instance;
}

void StateManager::CloseGame(){
	preparedStatus = STATE_CLOSE_GAME;
}

bool StateManager::Update(){
	bool end = false;
	// En el caso de que haya un estado preparado lo cambiamos
	if(preparedStatus != WITHOUT_STATE){
		LoadState(preparedStatus, &end);
		preparedStatus = WITHOUT_STATE;
	}

	if(!end){	// Aun no se ha llamado para cerrar el juego
		end = currentState->Input();
		currentState->Update(deltaTime);
		currentState->Draw();
	}

	UpdateDelta();

	return !end;
}

void StateManager::PrepareStatus(State_Code status){
	preparedStatus  = status;
}

void StateManager::LoadState(State_Code code, bool* end){
	if(currentState!=NULL){
		delete currentState;
		currentState = NULL;
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
		case STATE_NETGAME_CLIENT:
			if(!resourcesLoaded){
				resourcesLoaded = true;
				ResourceManager::LoadResources();
			}
			currentState =  new MultiPlayerGame();
			break;
		case STATE_NETGAME_SERVER:
			if(!resourcesLoaded){
				resourcesLoaded = true;
				ResourceManager::LoadResources();
			}
			n_engine->StartServer();
			currentState =  new MultiPlayerGame();
			break;
		case STATE_CLOSE_GAME:
			*end = true;
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