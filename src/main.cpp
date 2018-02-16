#include <iostream>
#include <StateCodes.h>
#include <NetworkStructs.h>
#include "Managers/StateManager.h"

ServerInfo ArgumentManager(int argc, char* argv[]){
	ServerInfo serverInfo;
	
	if(argc >= 2){
		if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--server") == 0){
			serverInfo.isServer = true;
		}

		else if(strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--ingame") == 0){
			serverInfo.isServer = true;
			serverInfo.inGameServer = true;
		}

		if(argc >= 3) serverInfo.serverName = argv[2];
	}

	return serverInfo;
}

int main(int argc, char* argv[]) {
	ServerInfo serverInfo = ArgumentManager(argc, argv);
	StateManager* stateManager = StateManager::GetInstance(&serverInfo);

	while(stateManager->Update()){}
	delete stateManager;

	return 0;
}