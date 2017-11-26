#include "NetworkObject.h"

NetworkObject::NetworkObject(bool isServer, int id){
	serverObject = isServer;
	objectId = id;
}

NetworkObject::~NetworkObject(){

}

/*
// ####### MENSAJES (Object to Server) #######
//
// Player	ID 		--> Quien lo envia
// Object	ID 		--> Que objeto debe recibirlo
// Message	ID 		--> Que tipo de variable se envia
// Variable	ID 		--> Que variable esta cambiando
// Variable	Value	--> El nuevo valor de la variable del objeto
*/

void NetworkObject::SendPackage(RakNet::BitStream bitstream, PacketPriority priority, PacketReliability reliability, RakNet::AddressOrGUID guid){
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	peer->Send(&bitstream, priority, reliability, 0, guid, true);
}

void NetworkObject::RecievePackages(){
}

void NetworkObject::SetIntVar(int k, int v){
	intVariables[k] = v;
}

void NetworkObject::SetFloatVar(int k, float v){
	floatVariables[k] = v;
}

void NetworkObject::SetVecIVar(int k, vector3di v){
	v3iVariables[k] = v;
}

void NetworkObject::SetVecFVar(int k, vector3df v){
	v3fVariables[k] = v;
}

int NetworkObject::GetIntVar(int k){
	int toRet = -1;
	std::map<int, int>::iterator i = intVariables.find(k);
	if (i != intVariables.end()) toRet = i->second;
	return(toRet);
}

float NetworkObject::GetFloatVar(int k){
	float toRet = -1;
	std::map<int, float>::iterator i = floatVariables.find(k);
	if (i != floatVariables.end()) toRet = i->second;
	return(toRet);
}

vector3di NetworkObject::GetVecIVar(int k){
	vector3di toRet = vector3di(-1, -1,-1);
	std::map<int, vector3di>::iterator i = v3iVariables.find(k);
	if (i != v3iVariables.end()) toRet = i->second;
	return(toRet);
}

vector3df NetworkObject::GetVecFVar(int k){
	vector3df toRet = vector3df(-1, -1,-1);
	std::map<int, vector3df>::iterator i = v3fVariables.find(k);
	if (i != v3fVariables.end()) toRet = i->second;
	return(toRet);
}