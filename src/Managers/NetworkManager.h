#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <iostream>
#include <map>

class NetworkObject;

class NetworkManager{
public:

	static NetworkManager* GetInstance();
	~NetworkManager();
	void InitObject();
	void EmptyObject();

	void Update();
	NetworkObject* GetMultiGame();

private:
	NetworkObject* multiGame;

	std::map<int, NetworkObject*> networkObjects;			// Map of Network Objects
	std::map<int, NetworkObject*> newNetworkObjects;		// Map of Network Objects not spawned
	std::map<int, NetworkObject*> toEraseNetworkObjects;	// Map of Network Objects to be erased
	std::map<int, NetworkObject*> spawnedObjects;			// Map of Network Objects already spawned

	void RetrieveObjects();
	void SpawnNewObjects();
	void EraseObjects();

	NetworkManager();
	NetworkManager(NetworkManager&);
	NetworkManager operator=(NetworkManager&);

};

#endif