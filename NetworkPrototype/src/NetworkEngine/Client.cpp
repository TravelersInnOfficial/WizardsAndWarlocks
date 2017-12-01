#include "Client.h"

Client::Client(std::string serverIp, int serverPort){
	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1, &descriptor, 1);
	peer->Connect(serverIp.c_str(), serverPort, 0, 0);
}

Client::~Client(){
	// Delete all network objects
	std::map<int, NetworkObject*>::iterator i = networkObjects.begin();
	for(;i!=networkObjects.end(); i++) delete i->second;

	// Destroy the PEER interface
	SendShutdown();
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Client::SendPackage(RakNet::BitStream* bitstream, PacketPriority priority, PacketReliability reliability){
	peer->Send(bitstream, priority, reliability, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Client::SendShutdown(){
	peer->Shutdown(300);
}

void Client::AddPlayer(int id, RakNet::RakNetGUID guid){
	bool exsits = false;
	for (auto &row : networkPlayers) {
		if(row.first == id || row.second == guid) {
			exsits = true;
			break;
		}
	}
	if(!exsits) networkPlayers[id] = guid;
}

void Client::RemovePlayer(int id){
	networkPlayers.erase(id);
}

void Client::CreateNetworkObject(int id, ObjectType type){
	networkObjects[id] = newNetworkObjects[id] = new NetworkObject(id, type);
}

void Client::RemoveNetworkObject(int id){
	networkObjects.erase(id);
}

std::map<int, NetworkObject*> Client::GetNetworkObjects(){
	return(networkObjects);
}

std::map<int, NetworkObject*> Client::GetNewNetworkObjects(){
	std::map<int, NetworkObject*> toRet = newNetworkObjects;
	std::map<int, NetworkObject*> emptyMap;
	newNetworkObjects = emptyMap;
	return(toRet);
}

void Client::RecievePackages(){

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {

			// CUANDO UN JUGADOR SE CONECTA
			case ID_PLAYER_JOIN:
			case ID_EXISTING_PLAYER:{
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int id;
				RakNet::RakNetGUID guid;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(id);
				bitstream.Read(guid);
				AddPlayer(id,guid);
				break;
			}

			// CUANDO UN JUGADOR SE DESCONECTA
			case ID_PLAYER_DISCONNECT: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int id;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(id);
				RemovePlayer(id);
				break;
			}

			// CUANDO SE PIERDE LA CONEXION CON EL SERVER
			case ID_CONNECTION_LOST: {
				exit(0);
				break;
			}

			// CUANDO SE CREA/ELIMINA/MODIFICA UN OBJETO
			case ID_OBJECT_STATUS_CHAGED: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				ModifyObject(&bitstream);
				break;
			}
		}
	}
}

void Client::ModifyObject(RakNet::BitStream* bitstream){
	VariableMapID messageSubId;
	bitstream->IgnoreBytes(sizeof(RakNet::MessageID));
	bitstream->Read(messageSubId);

	switch (messageSubId) {
		case ID_CREATE: {
			int k = -1;
			ObjectType o = ID_NO_OBJ;
			bitstream->Read(k);
			bitstream->Read(o);
			CreateNetworkObject(k, o);
			break;
		}
		case ID_EXISTING_OBJECT: {
			int k = -1;
			ObjectType o = ID_NO_OBJ;
			bitstream->Read(k);
			bitstream->Read(o);
			CreateNetworkObject(k, o);
			break;
		}
		case ID_REMOVE: {
			int k = -1;
			bitstream->Read(k);
			RemoveNetworkObject(k);
			break;
		}
		case ID_CHANGE_BOOL: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			bool v = false;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL){
				networkObjects[k]->SetBoolVar(k_var, v, false, false);
			}
			break;
		}
		case ID_CHANGE_INT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			int v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL){
				networkObjects[k]->SetIntVar(k_var, v, false, false);
			}
			break;
		}
		case ID_CHANGE_FLOAT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			float v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL){
				networkObjects[k]->SetFloatVar(k_var, v, false, false);
			}
			break;
		}
		case ID_CHANGE_VECINT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3di v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL){
				networkObjects[k]->SetVecIVar(k_var, v, false, false);
			}
			break;
		}
		case ID_CHANGE_VECFLOAT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3df v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL){
				networkObjects[k]->SetVecFVar(k_var, v, false, false);
			}
			break;
		}
	}

}

// Object variable messages
void Client::SetObjectBool(int objectId, ObjectVariable k, bool v){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_BOOL);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}

void Client::SetObjectInt(int objectId, ObjectVariable k, int v){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_INT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}

void Client::SetObjectFloat(int objectId, ObjectVariable k, float v){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_FLOAT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}

void Client::SetObjectIntVec(int objectId, ObjectVariable k, vector3di v){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_VECINT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}

void Client::SetObjectFloatVec(int objectId, ObjectVariable k, vector3df v){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_VECFLOAT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}