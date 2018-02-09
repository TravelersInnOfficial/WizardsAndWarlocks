#include "NetSeeker.h"

NetSeeker::NetSeeker(float timeToUpdateList){
	this->timeToUpdateList = timeToUpdateList;
	currentTimeLeft = 0;

	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1, &descriptor, 1);
	peer->SetTimeoutTime(15000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	Update(0);
}

NetSeeker::~NetSeeker(){
	SendShutdown();
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void NetSeeker::SendShutdown(){
	peer->Shutdown(300);
}

// Contar tiempo para actualizar lista
void NetSeeker::Update(float deltaTime){
	if(currentTimeLeft <= 0){
		Recieve();
		Ask();
		currentTimeLeft = timeToUpdateList;
	}
	currentTimeLeft -= deltaTime;
}

// Preguntar por las IPs de los juegos en LAN
void NetSeeker::Ask(){
	peer->Ping("255.255.255.255", 60000, false);
}

// Llamar a cada sec para actualizar las IP y las que no salgan, eliminarlas de la lista
void NetSeeker::Recieve(){
	std::vector<ServerData> newServers;

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {
			case ID_UNCONNECTED_PONG: {

				// READ BITSTREAM (data = INICIO, dataLength = FINAL)
				unsigned char* data = packet->data + sizeof(unsigned char) + sizeof(RakNet::TimeMS);
				unsigned int dataLength = packet->length - sizeof(unsigned char) - sizeof(RakNet::TimeMS);

				// READ THE IP
				std::string ip = packet->systemAddress.ToString();
				ip = TreatIp(ip);

				// READ STATE
				bool lobbyState = false;
				if(data[0] == '1') lobbyState = true;

				// READ NUMBER OF PLAYERS
				//int playerCount = atoi(data[1]);
				int playerCount = (int)(data[1] - '0');
				std::cout<<playerCount<<std::endl;

				// READ NAME

				// SET SERVER DATA
				ServerData newServerData;
				newServerData.ip = ip;
				newServerData.lobbyState = lobbyState;
				newServerData.playerCount = playerCount;

				newServers.push_back(newServerData);
				break;
			}

			default:{ break; }
		}
	}

	RegisterServers(newServers);
}

// Guardarlas en la lista, comprobar si existen e intentar mantener el mismo orden
void NetSeeker::RegisterServers(std::vector<ServerData> newServers){

	// Actualizamos los viejos eliminando los que ya no existan
	for(int i = 0; i < servers.size(); i++){
		int pos = Find(servers[i], newServers);
		if(pos == -1) servers.erase(servers.begin() + i);
		else{
			servers.at(i) = newServers.at(pos);
			newServers.erase(newServers.begin() + pos);
		}
	}
	
	// Metemos los nuevos
	for(int i = 0; i < newServers.size(); i++){
		int pos = Find(newServers[i], servers);
		if(pos == -1) servers.push_back(newServers[i]);
	}

}

// Buscamos los strings en un vector de string
int NetSeeker::Find(ServerData toFind, std::vector<ServerData> where){
	int toRet = -1;
	
	for(int i = 0; i < where.size() && toRet == -1; i++){
		int found = strcmp(toFind.ip.c_str(), where.at(i).ip.c_str());
		if(found == 0) toRet = i;
	}

	return toRet;
}

// Tratar la IP para quitar el puerto, que no nos interesa
std::string NetSeeker::TreatIp(std::string ip){
	ip.erase(ip.find("|"));
	return ip;
}

std::vector<ServerData> NetSeeker::GetList(){
	return(servers);
}