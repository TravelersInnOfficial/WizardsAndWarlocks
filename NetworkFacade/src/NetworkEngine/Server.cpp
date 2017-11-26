#include "Server.h"


Server::Server(int serverPort, int maxClients){
	peer = RakNet::RakPeerInterface::GetInstance();
	descriptor = RakNet::SocketDescriptor(serverPort, 0);
	peer->Startup(maxClients, &descriptor, 1);
	peer->SetMaximumIncomingConnections(maxClients);
	std::cout << "# Server started #"<<std::endl;
}

Server::~Server(){
	std::cout << "# Disconnecting Server #"<<std::endl;

	// Delete all network objects
	std::map<int, NetworkObject*>::iterator i = networkObjects.begin();
	for(;i!=networkObjects.end(); i++) delete i->second;

	// Destroy the PEER interface
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Server::SendPackage(RakNet::BitStream bitstream, PacketPriority priority, PacketReliability reliability, RakNet::AddressOrGUID guid){
	peer->Send(&bitstream, priority, reliability, 0, guid, true);
}

void Server::RecievePackages(){
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {
			case ID_NEW_INCOMING_CONNECTION: {
				lastPlayerId += 1;
				networkPlayers[lastPlayerId] = packet->guid;
				std::cout << "Se ha conectado el Player " << lastPlayerId << ": " << &(packet->guid) << std::endl;
				for (auto &row : networkPlayers) {
					RakNet::SystemAddress addr = peer->GetSystemAddressFromGuid(row.second);
					std::cout << "PLAYER: " << row.first << " has Addr: " << addr.ToString() << std::endl;
				}
				break;
			}
			case ID_CONNECTION_LOST: 
			case ID_DISCONNECTION_NOTIFICATION: {
				for (auto &row : networkPlayers) {
					if(row.second == packet->guid) networkPlayers.erase(row.first);
					std::cout << "PLAYER: " << row.first << " Se ha desconectado." << std::endl;
				}
				break;
			}
		}
	}
}

int Server::CreateNetworkObject(){
	lastObjectId += 1;
	networkObjects[lastObjectId] = new NetworkObject(true, lastObjectId);
	return (lastObjectId);
}