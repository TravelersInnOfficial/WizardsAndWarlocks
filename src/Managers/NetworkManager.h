#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <iostream>
#include <cstdlib>
#include <map>
#include <NetworkStructs.h>
#include "./../NetworkEngine/NetworkEngine.h"
#include "./../NetworkEngine/Server.h"
#include "./../NetworkEngine/Client.h"
#include "./PlayerManager.h"
#include "./../Players/Player.h"


class NetworkManager{

public:

	static NetworkManager* GetInstance();
	~NetworkManager();

	void Update();

private:

	std::map<int, NetworkObject*> networkObjects;			// Map of Network Objects
	std::map<int, NetworkObject*> newNetworkObjects;		// Map of Network Objects not spawned
	std::map<int, NetworkObject*> toEraseNetworkObjects;	// Map of Network Objects to be erased
	std::map<int, NetworkObject*> spawnedObjects;			// Map of Network Objects already spawned
	static NetworkManager* instance;

	NetworkManager();
	void RetrieveObjects();
	void SpawnNewObjects();
	void EraseObjects();
	void setPlayerManager(PlayerManager* playerManager);

};

#endif