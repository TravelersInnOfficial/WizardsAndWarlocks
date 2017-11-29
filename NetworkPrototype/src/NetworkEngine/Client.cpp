#include "Client.h"

Client::Client(std::string serverIp, int serverPort){
	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1, &descriptor, 1);
	peer->Connect(serverIp.c_str(), serverPort, 0, 0);
	std::cout << "# Client Started #"<<std::endl;
}

Client::~Client(){
	std::cout << "# Disconnecting Client #"<<std::endl;

	// Delete all network objects
	std::map<int, NetworkObject*>::iterator i = networkObjects.begin();
	for(;i!=networkObjects.end(); i++) delete i->second;

	// Destroy the PEER interface
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Client::SendPackage(RakNet::BitStream* bitstream, PacketPriority priority, PacketReliability reliability){
	peer->Send(bitstream, priority, reliability, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Client::RecievePackages(){

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {
			case ID_PLAYER_JOIN:
			case ID_EXISTING_PLAYER:{
				std::cout << "# Nuevo Jugador Conectado #" << std::endl;
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int id;
				RakNet::RakNetGUID guid;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(id);
				bitstream.Read(guid);
				AddPlayer(id,guid);
				break;
			}
			case ID_PLAYER_DISCONNECT: {
				std::cout << "# Jugador Desconectado #" << std::endl;
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int id;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(id);
				RemovePlayer(id);
				break;
			}
			case ID_CONNECTION_LOST: {
				std::cout << "# Conexion Perdida #" << std::endl;
				exit(0);
				break;
			}
			case ID_OBJECT_STATUS_CHAGED: {
				std::cout << "# Estado de Objeto Cambiado #" << std::endl;
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
			std::cout<<"Creamos el objeto"<<std::endl;
			int k = -1;
			ObjectType o;
			bitstream->Read(k);
			bitstream->Read(o);
			CreateNetworkObject(k, o);
			break;
		}
		case ID_EXISTING_OBJECT: {
			std::cout<<"Creamos el objeto existente"<<std::endl;
			int k = -1;
			ObjectType o;
			bitstream->Read(k);
			bitstream->Read(o);
			CreateNetworkObject(k, o);
			break;
		}
		case ID_REMOVE: {
			std::cout<<"Eliminamos el objeto"<<std::endl;
			int k = -1;
			bitstream->Read(k);
			RemoveNetworkObject(k);
			break;
		}
		case ID_CHANGE_BOOL: {
			std::cout<<"Bool Changed"<<std::endl;
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			bool v = false;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetBoolVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_INT: {
			std::cout<<"Int Changed"<<std::endl;
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			int v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetIntVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_FLOAT: {
			std::cout<<"Float Changed"<<std::endl;
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			float v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetFloatVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_VECINT: {
			std::cout<<"Vec Int Changed"<<std::endl;
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3di v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetVecIVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_VECFLOAT: {
			std::cout<<"Vec Float Changed"<<std::endl;
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3df v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetVecFVar(k_var, v, false, false);
			break;
		}
	}

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
	bool exsits = false;
	for (auto &row : networkObjects) {
		if(row.first == id) {
			exsits = true;
			break;
		}
	}
	if(!exsits) networkObjects[id] = new NetworkObject(id, type);
}

void Client::RemoveNetworkObject(int id){
	networkObjects.erase(id);
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