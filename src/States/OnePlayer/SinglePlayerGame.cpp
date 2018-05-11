#include "SinglePlayerGame.h"

#include "Lobby.h"
#include "Match.h"

#include "./../AI/SenseManager/RegionalSenseManager.h"
#include "./../Managers/BulletManager.h"
#include "./../Managers/EffectManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/StateManager.h"
#include "./../Managers/TrapManager.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/MenuManager.h>
#include <PhysicsEngine/BulletEngine.h>
#include <SoundEngine/SoundSystem.h>

SinglePlayerGame::SinglePlayerGame(){
	spellManager 	= SpellManager::GetInstance();
	bulletManager 	= BulletManager::GetInstance();
	effectManager 	= EffectManager::GetInstance();
	objectManager	= ObjectManager::GetInstance();
	trapManager		= TrapManager::GetInstance();
	senseManager	= RegionalSenseManager::GetInstance();
	playerManager	= PlayerManager::GetInstance();

	g_engine		= GraphicEngine::getInstance();
	f_engine		= BulletEngine::GetInstance();
	s_engine 		= SoundSystem::getInstance();

	m_stateGame 	= new Lobby(this);

	debug 		= false;
	mute 		= false;
	captured 	= false;

	m_changeMode	= 0;

	CreateSoundEvents();
}

SinglePlayerGame::~SinglePlayerGame(){
	if(m_stateGame!=nullptr) delete m_stateGame;

	playerManager->EmptyObject();
	senseManager->EmptyObject();
	trapManager->EmptyObject();
	objectManager->EmptyObject();
	effectManager->EmptyObject();
	bulletManager->EmptyObject();
	spellManager->EmptyObject();	// Tiene que eliminarse despues de el playerManager NECESARIO

	// Eliminamos las habitaciones
	g_engine->CleanRooms();

	// Liberamos los sonidos que teniamos puestos
	std::map<std::string, SoundEvent*>::iterator it = soundEvents.begin();
	for(; it!=soundEvents.end(); it++){
		SoundEvent* even = it->second;
		even->release();
		delete even;
	}
}

void SinglePlayerGame::StartGame(){
	m_changeMode = 1;
}

void SinglePlayerGame::ReturnLobby(){
	m_changeMode = 2;
}


void SinglePlayerGame::ChangeMode(){
	switch(m_changeMode){
		case 1:
			delete m_stateGame;						// Eliminamos el estado que haya actualmente
			CleanGame();							// Limpiamos el juego
			playerManager->SpawnAI();
			m_stateGame = new Match(this);			// Cargamos la partida
			playerManager->InitGame();				// Posicionamos los jugadores
			m_changeMode = 0;
			break;
		case 2:
			delete m_stateGame;
			CleanGame();
			playerManager->EraseAI();
			m_stateGame = new Lobby(this);
			playerManager->RestartMatchStatus();
			m_changeMode = 0;
			break;
	}
}

void SinglePlayerGame::CleanGame(){
	// Limpiamos los objetos
	objectManager->ClearMap();
	// Limpiamos los proyectiles
	bulletManager->DeleteAllProyectiles();
	// Limpiamos las trampas
	trapManager->ClearTraps();
	// Eliminamos las habitaciones
	g_engine->CleanRooms();
	// Limpiamos los hechizos
	spellManager->ResetAllDieHechizo();

	// Si estaba algun menu activado lo desactivamos
	g_engine->ToggleCameraMovement(false);
	g_engine->ToggleMenu(false);
}

bool SinglePlayerGame::Input(){
	if(!g_engine->run()){
		return true;
	}

	if(g_engine->IsKeyPressed(Key_Escape)){
		StateManager::GetInstance()->PrepareStatus(STATE_MENU);
	}

	// DEBUG
	if(g_engine->IsKeyPressed(Key_F1)) debug =! debug;


	if(g_engine->IsKeyPressed(Key_F2)){
		float vol = 1;
		if(!mute) vol = 0;
		SoundSystem::getInstance()->setVolume(vol);
		mute = !mute;
	}

	if(g_engine->IsKeyPressed(Key_F3)){
		captured = !captured;
		g_engine->ToggleCameraMovement(captured);
		g_engine->setCursorVisible(captured);
	}

	return m_stateGame->Input();
}

void SinglePlayerGame::Update(float deltaTime){
	if(m_changeMode != 0) ChangeMode();
	m_stateGame->Update(deltaTime);
	MenuManager::GetInstance()->Update(deltaTime);
}

void SinglePlayerGame::Draw(){
	g_engine->BeginDraw();

	m_stateGame->Draw();
	MenuManager::GetInstance()->Draw();
	
	g_engine->EndDraw();
	if(debug){
		f_engine->DebugDrawWorld();
	}
}

/********************************************************************************************************
 ****************************************** SOUND FUNCTIONS *********************************************
 ********************************************************************************************************/

void SinglePlayerGame::CreateSoundEvents() {
	//Create the sound events
	SoundEvent* defeat  = s_engine->createEvent("event:/Music/Defeat");
	SoundEvent* victory = s_engine->createEvent("event:/Music/Victory");
	SoundEvent* ghosts  = s_engine->createEvent("event:/Ambience/Ghosts");
	SoundEvent* waterDrops  = s_engine->createEvent("event:/Ambience/Water Drops");
	
	//Store them at the map
	soundEvents["defeat"]  = defeat;
	soundEvents["victory"] = victory;
	soundEvents["ghosts"]  = ghosts;
	soundEvents["waterdrops"]  = waterDrops;
	
}
void SinglePlayerGame::PlayEvent(std::string event, vector3df pos) {
	s_engine->playEvent(soundEvents[event], pos);
}

void SinglePlayerGame::PlayEvent(std::string event) {
	s_engine->playEvent(soundEvents[event]);
}