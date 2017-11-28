#include "NetworkObject.h"
#include "NetworkEngine.h"
#include "Server.h"

NetworkObject::NetworkObject(int id, ObjectType type){
	objectId = id;
	objectType = type;
}

NetworkObject::~NetworkObject(){

}

void NetworkObject::SetBoolVar(ObjectVariable k, bool v, bool notify, bool expandClientChange){
	boolVariables[k] = v;
	if(notify){
		Server* server = NetworkEngine::GetInstance()->GetServer();
		if(server != NULL) server->SetObjectBool(objectId, k, v, expandClientChange);
		else{
			Client* client = NetworkEngine::GetInstance()->GetClient();
			if(client != NULL) client->SetObjectBool(objectId, k, v);
		}
	}
}

void NetworkObject::SetIntVar(ObjectVariable k, int v, bool notify, bool expandClientChange){
	intVariables[k] = v;
	if(notify){
		Server* server = NetworkEngine::GetInstance()->GetServer();
		if(server != NULL) server->SetObjectInt(objectId, k, v, expandClientChange);
		else{
			Client* client = NetworkEngine::GetInstance()->GetClient();
			if(client != NULL) client->SetObjectInt(objectId, k, v);
		}
	}
}

void NetworkObject::SetFloatVar(ObjectVariable k, float v, bool notify, bool expandClientChange){
	floatVariables[k] = v;
	if(notify){
		Server* server = NetworkEngine::GetInstance()->GetServer();
		if(server != NULL) server->SetObjectFloat(objectId, k, v, expandClientChange);
		else{
			Client* client = NetworkEngine::GetInstance()->GetClient();
			if(client != NULL) client->SetObjectFloat(objectId, k, v);
		}
	}
}

void NetworkObject::SetVecIVar(ObjectVariable k, vector3di v, bool notify, bool expandClientChange){
	v3iVariables[k] = v;
	if(notify){
		Server* server = NetworkEngine::GetInstance()->GetServer();
		if(server != NULL) server->SetObjectIntVec(objectId, k, v, expandClientChange);
		else{
			Client* client = NetworkEngine::GetInstance()->GetClient();
			if(client != NULL) client->SetObjectIntVec(objectId, k, v);
		}
	}
}

void NetworkObject::SetVecFVar(ObjectVariable k, vector3df v, bool notify, bool expandClientChange){
	v3fVariables[k] = v;
	if(notify){
		Server* server = NetworkEngine::GetInstance()->GetServer();
		if(server != NULL) server->SetObjectFloatVec(objectId, k, v, expandClientChange);
		else{
			Client* client = NetworkEngine::GetInstance()->GetClient();
			if(client != NULL) client->SetObjectFloatVec(objectId, k, v);
		}
	}
}

bool NetworkObject::GetBoolVar(ObjectVariable k){
	bool toRet = false;
	std::map<int, bool>::iterator i = boolVariables.find(k);
	if (i != boolVariables.end()) toRet = i->second;
	return(toRet);
}

int NetworkObject::GetIntVar(ObjectVariable k){
	int toRet = -1;
	std::map<int, int>::iterator i = intVariables.find(k);
	if (i != intVariables.end()) toRet = i->second;
	return(toRet);
}

float NetworkObject::GetFloatVar(ObjectVariable k){
	float toRet = -1;
	std::map<int, float>::iterator i = floatVariables.find(k);
	if (i != floatVariables.end()) toRet = i->second;
	return(toRet);
}

vector3di NetworkObject::GetVecIVar(ObjectVariable k){
	vector3di toRet = vector3di(-1, -1,-1);
	std::map<int, vector3di>::iterator i = v3iVariables.find(k);
	if (i != v3iVariables.end()) toRet = i->second;
	return(toRet);
}

vector3df NetworkObject::GetVecFVar(ObjectVariable k){
	vector3df toRet = vector3df(-1, -1,-1);
	std::map<int, vector3df>::iterator i = v3fVariables.find(k);
	if (i != v3fVariables.end()) toRet = i->second;
	return(toRet);
}