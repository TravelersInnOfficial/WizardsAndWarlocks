#include "Client.h"

Client::Client(std::string serverIp, int serverPort){
	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1, &descriptor, 1);
	peer->Connect(serverIp.c_str(), serverPort, 0, 0);
	std::cout << "# Client Started #"<<std::endl;
}

Client::~Client(){
	std::cout << "# Disconnecting Client #"<<std::endl;

	// Delete all network objects
	std::map<int, NetworkObject*>::iterator i = networkObjects.begin();
	for(;i!=networkObjects.end(); i++) delete i->second;

	// Destroy the PEER interface
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Client::SendPackage(RakNet::BitStream bitstream, PacketPriority priority, PacketReliability reliability){
	peer->Send(&bitstream, priority, reliability, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Client::RecievePackages(){
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		std::cout << "Paquete recibido" << std::endl;
	}
}

int Client::CreateNetworkObject(){
	return(-1);
}