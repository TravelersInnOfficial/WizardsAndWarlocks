#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>
#include <Alliance.h>
#include <TrapCodes.h>
#include <SpellCodes.h>
#include <NetworkStructs.h>
#include "RakNetIncludes.h"
#include "NetworkObject.h"

class Player;

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
	int CreateNetworkObject(ObjectType type, bool broadcast = true, RakNet::RakNetGUID guid = RakNet::UNASSIGNED_RAKNET_GUID);
	void RemoveNetworkObject(int id, bool broadcast = true);

	// Match Status
	void EndMatch(Alliance winnerAlliance);

	// We manage the players connected
	int AddPlayer(RakNet::RakNetGUID guid);
	int RemovePlayer(RakNet::RakNetGUID guid);

	// We manage the traps on the map
	void SetTrap(vector3df point,vector3df normal, int playerId, int trapId);
	void EraseTrap(int trapId, int playerAffectedId);

	std::map<int, NetworkObject*> GetNetworkObjects();
	std::map<int, NetworkObject*> GetNewNetworkObjects();
	std::map<int, NetworkObject*> GetToEraseNetworkObjects();

	// Send object state change
	void SetObjectBool(		int objectId, ObjectVariable k, bool v,			bool expandClientChange);
	void SetObjectInt(		int objectId, ObjectVariable k, int v,			bool expandClientChange);
	void SetObjectFloat(	int objectId, ObjectVariable k, float v,		bool expandClientChange);
	void SetObjectIntVec(	int objectId, ObjectVariable k, vector3di v,	bool expandClientChange);
	void SetObjectFloatVec(	int objectId, ObjectVariable k, vector3df v,	bool expandClientChange);
	void SetObjectString(	int objectId, ObjectVariable k, std::string v,	bool expandClientChange);

	// Sync player spells and traps
	void SetPlayerTrap(int networkId, TrapEnum trap, int usings);
	void SetPlayerSpell(int networkId, int spellPosition, SPELLCODE spell);

	// Sync Others
	void NotifyDoorInteracted(int doorPos);
	void NotifyPotionInteracted(int potionPos, Player* p);

private:
	
	int multiGameId;
	NetworkObject* multiGameObject;

	int lastObjectId = -1;									// ID of the last object created on the server
	std::map<int, NetworkObject*> networkObjects;			// Map of Network Objects on the server
	std::map<int, NetworkObject*> newNetworkObjects;		// Map of Network Objects on the server that haven't been retrieved
	std::map<int, NetworkObject*> toEraseNetworkObjects;	// Map of NO Objects to be erased

	int lastPlayerId = -1;								// ID of the last player connected to the server
	std::map<int, RakNet::RakNetGUID> networkPlayers;	// Map of Players conected to the server
	std::map<int, int> clientToPlayer;					// Client(k) To Player object(v) map

	RakNet::RakPeerInterface *peer;						// Peer Interface
	RakNet::Packet *packet;								// Packet to send and recieve
	RakNet::SocketDescriptor descriptor;				// Socket Descriptor

};

#endif