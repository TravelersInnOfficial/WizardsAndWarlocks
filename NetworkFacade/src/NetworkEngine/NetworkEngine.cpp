#include "NetworkEngine.h"

static NetworkEngine* instance;

NetworkEngine::NetworkEngine(){
	serverIp = "127.0.0.1";
	serverPort = 60000;
}

NetworkEngine* NetworkEngine::GetInstance(){
	if(instance == 0) instance = new NetworkEngine();
	return instance;
}

void NetworkEngine::StartServer(){ server = new Server(serverPort, maxClients); }
void NetworkEngine::StartClient(){ client = new Client(serverIp, serverPort); }

Server* NetworkEngine::GetServer(){ return(server); }
Client* NetworkEngine::GetClient(){ return(client); }

void NetworkEngine::SetPort(int newPort){ serverPort = newPort; }
int NetworkEngine::GetPort(){ return(serverPort); }

void NetworkEngine::SetIp(std::string newIp){ serverIp = newIp; }
std::string NetworkEngine::GetIp(){ return(serverIp); }

void NetworkEngine::EndService(){
	std::cout << "# Disconnecting #"<<std::endl;
	if(client != NULL) delete client;
	if(server != NULL) delete server;
}