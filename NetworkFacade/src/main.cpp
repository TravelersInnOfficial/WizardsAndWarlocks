#include <iostream>
#include <map>
#include <string>
#include "NetworkEngine/NetworkEngine.h"

NetworkEngine* networkEngine = NetworkEngine::GetInstance();

int main() {
	bool isServer = true;
	std::cout << "(C)lient or (S)erver: ";
	std::string input;
	std::cin >> input;
	if (input[0] == 'C' || input[0] == 'c') isServer = false;


	//################### SERVER ###################
	if (isServer) {
		networkEngine->StartServer();
		//std::cout<< networkEngine->CreateNetworkObject() << std::endl;
		while(1) networkEngine->GetServer()->RecievePackages();
	}


	//################### CLIENT ###################
	else {
		std::string ip;
		std::cout << "IP: "; std::cin >> ip;
		networkEngine->SetIp(ip);
		networkEngine->StartClient();
		while(1) networkEngine->GetClient()->RecievePackages();
	}


	networkEngine->EndService();
	return 0;
}