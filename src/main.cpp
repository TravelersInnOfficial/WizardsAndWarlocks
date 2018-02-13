#include <iostream>
#include <StateCodes.h>
#include "./Managers/StateManager.h"

void ArgumentManager(int argc, char* argv[], bool* isServer, bool* ingameServer){	
	if(argc >= 2){
		if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--server") == 0){
			*isServer = true;
		}

		else if(strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--ingame") == 0){
			*isServer = true;
			*ingameServer = true;
		}
	}
}

int main(int argc, char* argv[]) {
	bool isServer = false;
	bool ingameServer = false;
	ArgumentManager(argc, argv, &isServer, &ingameServer);

	StateManager* stateManager = StateManager::GetInstance(isServer, ingameServer);
	while(stateManager->Update()){}
	
	delete stateManager;
	return 0;
}