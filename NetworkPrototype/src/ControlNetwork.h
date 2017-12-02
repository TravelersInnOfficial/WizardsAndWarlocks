#ifndef CONTROLNETWORK_H
#define CONTROLNETWORK_H

#include <iostream>
#include <cstdlib>
#include <map>
#include "NetworkEngine/NetworkEngine.h"
#include "NetworkEngine/NetworkStructs.h"
#include "NetworkEngine/Server.h"
#include "NetworkEngine/Client.h"
#include "./ControlPlayer.h"
#include "./Player.h"


class ControlNetwork{

public:

	static ControlNetwork* GetInstance();
	~ControlNetwork();

	void Update();

private:

	std::map<int, NetworkObject*> networkObjects;			// Map of Network Objects
	std::map<int, NetworkObject*> newNetworkObjects;		// Map of Network Objects not spawned
	std::map<int, NetworkObject*> toEraseNetworkObjects;	// Map of Network Objects to be erased
	std::map<int, NetworkObject*> spawnedObjects;			// Map of Network Objects already spawned
	static ControlNetwork* instance;

	ControlNetwork();
	void RetrieveObjects();
	void SpawnNewObjects();
	void EraseObjects();
	void setPlayerManager(ControlPlayer* playerManager);

};

#endif