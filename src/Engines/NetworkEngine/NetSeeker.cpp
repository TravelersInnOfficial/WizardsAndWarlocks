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
	std::vector<std::string> ips;

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {
			case ID_UNCONNECTED_PONG: {
				std::string ip = packet->systemAddress.ToString();
				ips.push_back(TreatIp(ip));
				break;
			}

			default:{
				break;
			}
		}
	}

	RegisterIp(ips);	// Registramos todas las IP
}

// Guardarlas en la lista, comprobar si existen e intentar mantener el mismo orden
void NetSeeker::RegisterIp(std::vector<std::string> ips){

	// Eliminamos los que ya no existan
	// Si no lo encontramos, no existe asi que lo eliminamos
	for(int i = 0; i < lobbys.size(); i++){
		int pos = Find(lobbys[i], ips);
		if(pos == -1) lobbys.erase(lobbys.begin() + i);
	}
	
	// Metemos los nuevos
	// Si no lo encontramos, no existe asi que lo creamos
	for(int i = 0; i < ips.size(); i++){
		int pos = Find(ips[i], lobbys);
		if(pos == -1) lobbys.push_back(ips[i]);
	}

}

// Buscamos los strings en un vector de string
int NetSeeker::Find(std::string toFind, std::vector<std::string> where){
	int toRet = -1;
	
	for(int i = 0; i < where.size() && toRet == -1; i++){
		int found = strcmp(toFind.c_str(), where.at(i).c_str());
		if(found == 0) toRet = i;
	}

	return toRet;
}

// Tratar la IP para quitar el puerto, que no nos interesa
std::string NetSeeker::TreatIp(std::string ip){
	ip.erase(ip.find("|"));
	return ip;
}

std::vector<std::string> NetSeeker::GetList(){
	return(lobbys);
}