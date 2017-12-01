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

	void SendShutdown();

	// Sends game-level packages
	void SendPackage(RakNet::BitStream*, PacketPriority, PacketReliability, RakNet::AddressOrGUID, bool broadcast);

	// Reads game-level packages
	void RecievePackages();

	// We manage the Object-Level packages
	void ModifyObject(RakNet::BitStream* bitstream);

	// We manage a network object and notify the clients
	int CreateNetworkObject(ObjectType type);
	void RemoveNetworkObject(int id);

	// We manage the players connected
	int AddPlayer(RakNet::RakNetGUID guid);
	int RemovePlayer(RakNet::RakNetGUID guid);

	std::map<int, NetworkObject*> GetNetworkObjects();
	std::map<int, NetworkObject*> GetNewNetworkObjects();

	// Send object state change
	void SetObjectBool(		int objectId, ObjectVariable k, bool v,			bool expandClientChange);
	void SetObjectInt(		int objectId, ObjectVariable k, int v,			bool expandClientChange);
	void SetObjectFloat(	int objectId, ObjectVariable k, float v,		bool expandClientChange);
	void SetObjectIntVec(	int objectId, ObjectVariable k, vector3di v,	bool expandClientChange);
	void SetObjectFloatVec(	int objectId, ObjectVariable k, vector3df v,	bool expandClientChange);

private:
	
	int lastObjectId = -1;								// ID of the last object created on the server
	std::map<int, NetworkObject*> networkObjects;		// Vector of Network Objects on the server
	std::map<int, NetworkObject*> newNetworkObjects;	// Vector of Network Objects on the server that haven't been retrieved

	int lastPlayerId = -1;								// ID of the last player connected to the server
	std::map<int, RakNet::RakNetGUID> networkPlayers;	// Vector of Players conected to the server

	RakNet::RakPeerInterface *peer;						// Peer Interface
	RakNet::Packet *packet;								// Packet to send and recieve
	RakNet::SocketDescriptor descriptor;				// Socket Descriptor

};

#endif