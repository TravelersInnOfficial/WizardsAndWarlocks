#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstdlib>
#include <map>
#include "RakNetIncludes.h"
#include "NetworkStructs.h"
#include "NetworkObject.h"

class Client{

public:

	// We pass the IP we're gonna connect to 
	// And the port we're talking to
	Client(std::string serverIp = "127.0.0.1", int serverPort = 6000);
	~Client();

	// Sends game-level packages
	void SendPackage(RakNet::BitStream*, PacketPriority, PacketReliability);

	// Reads game-level packages
	void RecievePackages();

	// We manage the Object-Level packages
	void ModifyObject(RakNet::BitStream* bitstream);

	// We send a shoutdown to the server notifying we're disconnecting
	void SendShutdown();

	// We manage the network objects from server messages
	void CreateNetworkObject(int id, ObjectType type);
	void RemoveNetworkObject(int id);

	// We manage the players connected
	void AddPlayer(int id, RakNet::RakNetGUID guid);
	void RemovePlayer(int id);

	std::map<int, NetworkObject*> GetNetworkObjects();
	std::map<int, NetworkObject*> GetNewNetworkObjects();

	// Send object state change
	void SetObjectBool(		int objectId, ObjectVariable k, bool v);
	void SetObjectInt(		int objectId, ObjectVariable k, int v);
	void SetObjectFloat(	int objectId, ObjectVariable k, float v);
	void SetObjectIntVec(	int objectId, ObjectVariable k, vector3di v);
	void SetObjectFloatVec(	int objectId, ObjectVariable k, vector3df v);

private:
	
	std::map<int, NetworkObject*> networkObjects;		// Vector of Network Objects on the Client
	std::map<int, NetworkObject*> newNetworkObjects;	// Vector of Network Objects on the Client that haven't been retrieved
	std::map<int, RakNet::RakNetGUID> networkPlayers;	// Vector of Players conected to the Server
	int player_id;										// Our PLAYER_ID
	
	RakNet::RakPeerInterface *peer;						// Peer Interface
	RakNet::Packet *packet;								// Packet to send and recieve
	RakNet::SocketDescriptor descriptor;				// Socket Descriptor

};

#endif