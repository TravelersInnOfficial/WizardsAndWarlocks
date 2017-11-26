#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include <iostream>
#include <map>
#include "RakNetIncludes.h"
#include "../vector3d.h"

class NetworkObject{

public:

	// We create a network object with an ID
	NetworkObject(bool, int);
	~NetworkObject();

	// Setters will send info to the server
	void SetIntVar(int, int);
	void SetFloatVar(int, float);
	void SetVecIVar(int, vector3di);
	void SetVecFVar(int, vector3df);

	// Getters will retrieve info from the network object (Server must update it)
	int GetIntVar(int);
	float GetFloatVar(int);
	vector3di GetVecIVar(int);
	vector3df GetVecFVar(int);

	// Send and recieve object-level packages
	void SendPackage(RakNet::BitStream, PacketPriority, PacketReliability, RakNet::AddressOrGUID);
	void RecievePackages();

private:

	// The behaviour changes depending on where it's running
	bool serverObject;

	// Game object ID, must be the same on all clients & on the server
	int objectId;

	// Maps that will store all our info
	std::map<int, int> intVariables;
	std::map<int, float> floatVariables;
	std::map<int, vector3di> v3iVariables;
	std::map<int, vector3df> v3fVariables;

};

#endif