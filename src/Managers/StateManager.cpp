#include "StateManager.h"
#include <time.h>

StateManager* StateManager::instance = 0;

StateManager::StateManager(ServerInfo* serverInfo){
	if(serverInfo == NULL){
		ServerInfo dummyInfo;
		serverInfo = &dummyInfo;
		serverInfo->isServer = false;
		serverInfo->inGameServer = false;
		serverInfo->serverName = "";
	}

	// DeltaTime
	timeStart = 0.0f;
	deltaTime = 0.0f;
	minFrameTime = (1.0f / 240.0f) * 1000;

	// Engines
	g_engine = GraphicEngine::getInstance(serverInfo->inGameServer);
	f_engine = BulletEngine::GetInstance();
	f_engine->CreateWorld();
	s_engine = SoundSystem::getInstance();
	s_engine->createSystem("./../assets/banks/");
	n_engine = NetworkEngine::GetInstance(serverInfo);

	srand(time(0));
	currentState = NULL;
	
	State_Code firstState = STATE_MENU;
	if(serverInfo->isServer) firstState = STATE_NETGAME_SERVER;
	
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

StateManager* StateManager::GetInstance(ServerInfo* serverInfo){
	if(instance==0) instance = new StateManager(serverInfo);
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

	// Esta cutremente aqui, SOLO ES PARA PRUEBAS
		std::string myFps = std::to_string(int(1/deltaTime));
		std::wstring wsTmp(myFps.begin(), myFps.end());
		g_engine->ChangeWindowName(wsTmp);

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
			n_engine->StartServer();
			if(!resourcesLoaded){
				resourcesLoaded = true;
				ResourceManager::LoadResources();
			}
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