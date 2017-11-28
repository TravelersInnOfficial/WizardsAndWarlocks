#include <iostream>
#include <unistd.h>
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
		//std::cout << networkEngine->GetServer()->CreateNetworkObject() << std::endl;
		while(1){
			//std::cin >> input;
			networkEngine->GetServer()->RecievePackages();
		}
	}


	//################### CLIENT ###################
	else {
		/*std::string ip;
		std::cout << "IP: "; std::cin >> ip;
		networkEngine->SetIp(ip);*/
		networkEngine->StartClient();
		//sleep(5);
		//std::cout<<"Enviamos shutdown"<<std::endl;
		//networkEngine->GetClient()->SendShutdown();
		while(1) networkEngine->GetClient()->RecievePackages();
	}


	networkEngine->EndService();
	return 0;
}