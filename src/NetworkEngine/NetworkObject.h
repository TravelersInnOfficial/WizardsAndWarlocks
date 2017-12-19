#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include <iostream>
#include <map>
#include <vector3d.h>
#include <NetworkStructs.h>
#include "RakNetIncludes.h"

class NetworkObject{

public:

	// We create a network object with an ID and a type
	NetworkObject();
	NetworkObject(int id, ObjectType type);
	~NetworkObject();

	// Returns the type of the object
	ObjectType GetObjType();
	int GetObjId();

	// Setters will send info to the server
	void SetBoolVar(	ObjectVariable k, bool v,		bool notify, bool expandClientChanges);
	void SetIntVar(		ObjectVariable k, int v,		bool notify, bool expandClientChanges);
	void SetFloatVar(	ObjectVariable k, float v,		bool notify, bool expandClientChanges);
	void SetVecIVar(	ObjectVariable k, vector3di v, 	bool notify, bool expandClientChanges);
	void SetVecFVar(	ObjectVariable k, vector3df v, 	bool notify, bool expandClientChanges);

	// Getters will retrieve info from the network object (Server must update it)
	bool GetBoolVar(ObjectVariable k);
	int GetIntVar(ObjectVariable k);
	float GetFloatVar(ObjectVariable k);
	vector3di GetVecIVar(ObjectVariable k);
	vector3df GetVecFVar(ObjectVariable k);

private:

	// Game object ID, must be the same on all clients & on the server
	int objectId;

	// Game object ID, must be the same on all clients & on the server
	ObjectType objectType;

	// Maps that will store all our info
	std::map<int, bool>			boolVariables;
	std::map<int, int>			intVariables;
	std::map<int, float>		floatVariables;
	std::map<int, vector3di>	v3iVariables;
	std::map<int, vector3df>	v3fVariables;

};

#endif