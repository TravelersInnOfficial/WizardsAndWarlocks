#include "NetworkEngine.h"

static NetworkEngine* instance = 0;

NetworkEngine::NetworkEngine(ServerInfo* serverInfo){
	if(serverInfo == NULL){
		ServerInfo dummyInfo;
		serverInfo = &dummyInfo;
		serverInfo->isServer = false;
		serverInfo->inGameServer = false;
		serverInfo->serverName = "";
	}

	serverIp = "127.0.0.1";
	serverPort = 60000;
	isServer = false;
	isClient = false;
	server = NULL;
	client = NULL;
	
	if(serverInfo->isServer) StartServer(serverInfo);
}

NetworkEngine* NetworkEngine::GetInstance(ServerInfo* serverInfo){
	if(instance == 0) instance = new NetworkEngine(serverInfo);
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

void NetworkEngine::StartServer(ServerInfo* serverInfo){
	if(serverInfo == NULL){
		ServerInfo dummyInfo;
		serverInfo = &dummyInfo;
		serverInfo->isServer = false;
		serverInfo->inGameServer = false;
		serverInfo->serverName = "";
	}

	if(!isClient && !isServer){
		isServer = true;
		server = new Server(serverPort, maxClients, serverInfo->inGameServer, serverInfo->serverName);
	}
}

void NetworkEngine::StartClient(bool proprietary){
	if(!isClient && !isServer){
		isClient = true;
		client = new Client(serverIp, serverPort, proprietary);
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