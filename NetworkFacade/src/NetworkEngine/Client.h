#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <map>
#include "RakNetIncludes.h"
#include "NetworkStructs.h"
#include "NetworkObject.h"

class Client{

public:

	// We pass the IP we're gonna connect to 
	// And the port we're talking to
	Client(std::string serverIp, int serverPort);
	~Client();

	// Sends game-level packages
	void SendPackage(RakNet::BitStream, PacketPriority, PacketReliability);

	// Reads game-level packages
	void RecievePackages();

	// We create an object in the Client and return its ID
	int CreateNetworkObject();

private:
	
	std::map<int, NetworkObject*> networkObjects;		// Vector of Network Objects on the Client
	std::map<int, RakNet::RakNetGUID> networkPlayers;	// Vector of Players conected to the Server

	RakNet::RakPeerInterface *peer;						// Peer Interface
	RakNet::Packet *packet;								// Packet to send and recieve
	RakNet::SocketDescriptor descriptor;				// Socket Descriptor

};

#endif