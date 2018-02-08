#include <iostream>
#include <StateCodes.h>
#include "./Managers/StateManager.h"

void ArgumentManager(int argc, char* argv[], bool* isServer){
	if(argc > 1){
		if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--server") == 0) *isServer = true;
	}
}

int main(int argc, char* argv[]) {
	bool isServer = false;
	ArgumentManager(argc, argv, &isServer);

	StateManager* stateManager = StateManager::GetInstance(isServer);
	while(stateManager->Update()){}
	
	delete stateManager;
	return 0;
}