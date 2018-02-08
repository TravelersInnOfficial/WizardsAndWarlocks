#include "StateManager.h"
#include <time.h>

StateManager* StateManager::instance = 0;

StateManager::StateManager(bool isServer, bool ingameServer){
	// DeltaTime
	timeStart = 0.0f;
	deltaTime = 0.0f;
	minFrameTime = (1.0f / 240.0f) * 1000;

	// Engines
	g_engine = GraphicEngine::getInstance(ingameServer);
	f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	s_engine = SoundSystem::getInstance();
	s_engine->createSystem("./../assets/banks/");
	n_engine = NetworkEngine::GetInstance(ingameServer);

	srand(time(0));
	currentState = NULL;
	
	State_Code firstState = STATE_MENU;
	if(isServer) firstState = STATE_NETGAME_SERVER;
	
	LoadState(firstState);
	preparedStatus = WITHOUT_STATE;
	resourcesLoaded = false;
}

StateManager::~StateManager(){
	if(currentState != NULL) delete currentState;
	delete f_engine;
	delete g_engine;
	delete s_engine;
	delete n_engine;
	instance = 0;
}

StateManager* StateManager::GetInstance(bool isServer, bool ingameServer){
	if(instance==0) instance = new StateManager(isServer, ingameServer);
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
		case WITHOUT_STATE:
			currentState = NULL;
			break;
	}
}

void StateManager::UpdateDelta(){

	// Capping FPS & Calculating deltaTime
	deltaTime = 0;
	timeStart = g_engine->getTime();
	float currentTime = timeStart;
	
	while(deltaTime < minFrameTime){
		deltaTime = currentTime - timeStart;
		g_engine->run();
		currentTime = g_engine->getTime();
	}

	deltaTime *= 0.001;
}