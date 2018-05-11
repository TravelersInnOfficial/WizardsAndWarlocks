#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <iostream>
#include <StateCodes.h>

class NetworkEngine;
class GraphicEngine;
class BulletEngine;
class MenuManager;
class SoundSystem;
struct ServerInfo;
class GUIEngine;
class State;

class StateManager{
public:
	static StateManager* GetInstance(ServerInfo* serverInfo = nullptr);

	~StateManager();

	void PrepareStatus(State_Code status);
	bool Update();
	void LoadState(State_Code code, bool* end = nullptr);
	void CloseGame();
	float GetDeltaTime();

	void SetLoadingStatusString(std::string status);
	int GetWizardAINumber();
	int GetWarlockAINumber();
	int* GetWizardAINumberPointer();
	int* GetWarlockAINumberPointer();

private:
	void UpdateDelta();

	State_Code 	preparedStatus;
	State*		currentState; 

	BulletEngine*	f_engine;
	GraphicEngine*	g_engine;
	SoundSystem*	s_engine;
	NetworkEngine*	n_engine;
	MenuManager* 	m_engine;
	GUIEngine* 		gui_engine;


	float deltaTime;
	float timeStart;
	float minFrameTime;

	int maxWizardPlayers;
	int maxWarlockPlayers;

	bool resourcesLoaded;
	bool loading;

	StateManager(ServerInfo* serverInfo = nullptr);
};

#endif