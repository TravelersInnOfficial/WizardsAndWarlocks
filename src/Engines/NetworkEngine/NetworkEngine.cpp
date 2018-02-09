#include "NetworkEngine.h"

static NetworkEngine* instance = 0;

NetworkEngine::NetworkEngine(bool serverCreatedFromGame){
	serverIp = "127.0.0.1";
	serverPort = 60000;
	isServer = false;
	isClient = false;
	server = NULL;
	client = NULL;
	if(serverCreatedFromGame) StartServer(serverCreatedFromGame);
}

NetworkEngine* NetworkEngine::GetInstance(bool serverCreatedFromGame){
	if(instance == 0) instance = new NetworkEngine(serverCreatedFromGame);
	return instance;
}

NetworkEngine::~NetworkEngine(){
	EndService();
	instance = 0;
}

void NetworkEngine::Update(float deltaTime, bool isLobby){
	if (isServer && server != NULL){
		server->Update(deltaTime);
		server->RecievePackages(isLobby);
	}
	if (isClient && client != NULL) client->RecievePackages();
}

void NetworkEngine::StartServer(bool serverCreatedFromGame){
	if(!isClient && !isServer){
		isServer = true;
		server = new Server(serverPort, maxClients, serverCreatedFromGame);
	}
}

void NetworkEngine::StartClient(){
	if(!isClient && !isServer){
		isClient = true;
		client = new Client(serverIp, serverPort);
	}
}

Server* NetworkEngine::GetServer(){ return(server); }
Client* NetworkEngine::GetClient(){ return(client); }

void NetworkEngine::SetPort(int newPort){ serverPort = newPort; }
int NetworkEngine::GetPort(){ return(serverPort); }

void NetworkEngine::SetIp(std::string newIp){ serverIp = newIp; }
std::string NetworkEngine::GetIp(){ return(serverIp); }

bool NetworkEngine::IsServerInit(){ return(isServer); }
bool NetworkEngine::IsClientInit(){ return(isClient); }

void NetworkEngine::EndService(){
	if(client != NULL){
		delete client;
		client = NULL;
		isClient = false;
	}
	if(server != NULL) {
		delete server;
		server = NULL;
		isServer = false;
	}
}