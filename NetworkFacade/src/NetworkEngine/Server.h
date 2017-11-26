#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include "RakNetIncludes.h"
#include "NetworkStructs.h"
#include "NetworkObject.h"

class Server{

public:

	// We pass the port we're listening to
	// And the max connections we allow simultaneously
	Server(int serverPort, int maxClients);
	~Server();

	// Sends game-level packages
	void SendPackage(RakNet::BitStream, PacketPriority, PacketReliability, RakNet::AddressOrGUID);

	// Reads game-level packages
	void RecievePackages();

	// We create an object in the server and return its ID
	int CreateNetworkObject();

private:
	
	int lastObjectId = -1;								// ID of the last object created on the server
	std::map<int, NetworkObject*> networkObjects;		// Vector of Network Objects on the server

	int lastPlayerId = -1;								// ID of the last player connected to the server
	std::map<int, RakNet::RakNetGUID> networkPlayers;	// Vector of Players conected to the server

	RakNet::RakPeerInterface *peer;						// Peer Interface
	RakNet::Packet *packet;								// Packet to send and recieve
	RakNet::SocketDescriptor descriptor;				// Socket Descriptor

};

#endif