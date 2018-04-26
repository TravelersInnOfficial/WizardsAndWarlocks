#include "StateManager.h"
#include <time.h>
#include <iostream>
#include <cstdio>
#include <thread>
#include <chrono>
#include <GraphicEngine/MenuManager.h>


StateManager::StateManager(ServerInfo* serverInfo){

	maxWizardPlayers = 4;
	maxWarlockPlayers = 4;

	if(serverInfo == nullptr){
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
	gui_engine = GUIEngine::GetInstance();
	m_engine = MenuManager::GetInstance();

	srand(time(0));
	currentState = nullptr;
	
	State_Code firstState = STATE_MENU;
	//State_Code firstState = STATE_GAME;
	if(serverInfo->isServer) firstState = STATE_NETGAME_SERVER;
	
	LoadState(firstState);
	preparedStatus = WITHOUT_STATE;
	resourcesLoaded = false;
	loading = false;
}

StateManager::~StateManager(){
	if(currentState != nullptr) delete currentState;
	//Engines are deleted automatically
}

StateManager* StateManager::GetInstance(ServerInfo* serverInfo){
	static StateManager instance = StateManager(serverInfo);
	return &instance;
}

float StateManager::GetDeltaTime(){
	return deltaTime;
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
	if(currentState!=nullptr){
		delete currentState;
		currentState = nullptr;
		loading = false;
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
			currentState = nullptr;
			break;
		case STATE_LOADING_SCREEN:
			loading = true;
			currentState = new LoadingScreen();	
			break;
	}
}

void StateManager::UpdateDelta(){
	using namespace std::chrono_literals;
	using clk = std::chrono::high_resolution_clock;
	static auto t = clk::now();

	// DECLARAMOS SPF y FPS
	constexpr auto fps = 200.0f;
	constexpr auto spf = 1000000000ns / fps;

	// CALCULAMOS DELTA TIME ANTES DE ESPERAR Y ESPERAMOS
	auto passed = clk::now() - t;
	if (passed < spf){
		auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(spf - passed - 100us);
		std::this_thread::sleep_for(ns);
	}

	// CALCULAMOS DELTA TIME
	passed = clk::now() - t;
	std::chrono::duration<double, std::milli> milisecondsPassed = passed;
	deltaTime = milisecondsPassed.count() / 1000.0f;
	if(deltaTime > 1.0f) deltaTime = 0.02f;

	// GUARDAMOS LA T
	t = clk::now();

	// Esta cutremente aqui, SOLO ES PARA PRUEBAS
	std::string myFps = std::to_string(int(1/(deltaTime)));
	g_engine->ChangeWindowName(myFps);
}

int StateManager::GetWizardAINumber(){
	return maxWizardPlayers;
}

int StateManager::GetWarlockAINumber(){
	return maxWarlockPlayers;
}

int* StateManager::GetWizardAINumberPointer(){
	return &maxWizardPlayers;
}

int* StateManager::GetWarlockAINumberPointer(){
	return &maxWarlockPlayers;
}

void StateManager::SetLoadingStatusString(std::string status){
	/*
	if(loading){
		LoadingScreen *s = (LoadingScreen*) currentState;
		s->SetStatusText(status);
	}
	*/
}
