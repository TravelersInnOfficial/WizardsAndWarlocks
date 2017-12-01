#include "Server.h"


Server::Server(int serverPort, int maxClients){
	peer = RakNet::RakPeerInterface::GetInstance();
	descriptor = RakNet::SocketDescriptor(serverPort, 0);
	peer->Startup(maxClients, &descriptor, 1);
	peer->SetMaximumIncomingConnections(maxClients);
}

Server::~Server(){
	// Delete all network objects
	std::map<int, NetworkObject*>::iterator i = networkObjects.begin();
	for(;i!=networkObjects.end(); i++) delete i->second;

	// Destroy the PEER interface
	SendShutdown();
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Server::SendShutdown(){
	peer->Shutdown(300);
}

void Server::SendPackage(RakNet::BitStream* bitstream, PacketPriority priority, PacketReliability reliability, RakNet::AddressOrGUID guid, bool broadcast){
	peer->Send(bitstream, priority, reliability, 0, guid, broadcast);
}

int Server::CreateNetworkObject(ObjectType type){
	lastObjectId += 1;
	networkObjects[lastObjectId] = new NetworkObject(lastObjectId, type);
	newNetworkObjects[lastObjectId] = networkObjects[lastObjectId];

	// We create the object on the server
	RakNet::BitStream bitstream;
	bitstream.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	bitstream.Write(ID_CREATE);
	bitstream.Write(lastObjectId);
	bitstream.Write(type);
	SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);

	return (lastObjectId);
}

void Server::RemoveNetworkObject(int id){
	// We delete the object on the server
	RakNet::BitStream bitstream;
	bitstream.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	bitstream.Write(ID_REMOVE);
	bitstream.Write(id);
	SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);

	// Eliminamos el local
	networkObjects.erase(id);
}

int Server::AddPlayer(RakNet::RakNetGUID guid){
	lastPlayerId += 1;
	networkPlayers[lastPlayerId] = guid;
	return(lastPlayerId);
}

int Server::RemovePlayer(RakNet::RakNetGUID guid){
	int id = -1;
	for (auto &row : networkPlayers) {
		if(row.second == guid) {
			networkPlayers.erase(row.first);
			id = row.first;
			break;
		}
	}
	return(id);
}

std::map<int, NetworkObject*> Server::GetNetworkObjects(){
	return(networkObjects);
}

std::map<int, NetworkObject*> Server::GetNewNetworkObjects(){
	std::map<int, NetworkObject*> toRet = newNetworkObjects;
	std::map<int, NetworkObject*> emptyMap;
	newNetworkObjects = emptyMap;
	return(toRet);
}

void Server::RecievePackages(){

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {

			// CUANDO SE CONECTA UN CLIENTE
			case ID_NEW_INCOMING_CONNECTION: {
				// Nos guardamos el nuevo cliente
				int id = AddPlayer(packet->guid);

				// Notificamos a todos los usuarios de que se ha conectado un player
				RakNet::BitStream bitstream;
				bitstream.Write((RakNet::MessageID)ID_PLAYER_JOIN);
				bitstream.Write(id);
				bitstream.Write(packet->guid);
				SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED,  RakNet::UNASSIGNED_RAKNET_GUID, true);

				// Creamos el nuevo jugador y lo enviamos a todos los clientes
				int newPlayerId = CreateNetworkObject(ID_PLAYER_O);

				// Notificamos al nuevo jugador de todos los jugadores existentes hasta el momento
				for (auto &rowClient : networkPlayers) {
					if (rowClient.second != packet->guid){
						RakNet::BitStream updateNewPlayerPlayers;
						updateNewPlayerPlayers.Write((RakNet::MessageID)ID_EXISTING_PLAYER);
						updateNewPlayerPlayers.Write(rowClient.first);
						updateNewPlayerPlayers.Write(rowClient.second);
						SendPackage(&updateNewPlayerPlayers, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					}
				}

				// Notificamos al nuevo jugador de todos los elementos existentes hasta el momento
				for (auto &rowObj : networkObjects) {
					if(rowObj.first != newPlayerId){
						RakNet::BitStream updateNewPlayerObjects;
						updateNewPlayerObjects.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
						updateNewPlayerObjects.Write(ID_EXISTING_OBJECT);
						updateNewPlayerObjects.Write(rowObj.first);
						updateNewPlayerObjects.Write(rowObj.second->GetObjType());
						SendPackage(&updateNewPlayerObjects, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					}
				}

				break;
			}

			// CUANDO SE DESCONECTA UN CLIENTE
			case ID_CONNECTION_LOST: 
			case ID_DISCONNECTION_NOTIFICATION: {
				// Delete the player from the server
				int id = RemovePlayer(packet->guid);

				// Send the disconnect message to all users
				RakNet::BitStream bitstream;
				bitstream.Write((RakNet::MessageID)ID_PLAYER_DISCONNECT);
				bitstream.Write(id);
				SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);

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

void Server::ModifyObject(RakNet::BitStream* bitstream){
	VariableMapID messageSubId;
	bitstream->IgnoreBytes(sizeof(RakNet::MessageID));
	bitstream->Read(messageSubId);

	switch (messageSubId) {
		case ID_CREATE: {
			break;
		}
		case ID_EXISTING_OBJECT: {
			break;
		}
		case ID_REMOVE: {
			break;
		}
		case ID_CHANGE_BOOL: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			bool v = false;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetBoolVar(k_var, v, true, true);
			break;
		}
		case ID_CHANGE_INT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			int v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetIntVar(k_var, v, true, true);
			break;
		}
		case ID_CHANGE_FLOAT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			float v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetFloatVar(k_var, v, true, true);
			break;
		}
		case ID_CHANGE_VECINT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3di v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetVecIVar(k_var, v, true, true);
			break;
		}
		case ID_CHANGE_VECFLOAT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3df v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			networkObjects[k]->SetVecFVar(k_var, v, true, true);
			break;
		}
	}

}

// Object variable messages
void Server::SetObjectBool(int objectId, ObjectVariable k, bool v, bool expandClientChange){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_BOOL);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	if(!expandClientChange) SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	else SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
}

void Server::SetObjectInt(int objectId, ObjectVariable k, int v, bool expandClientChange){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_INT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	if(!expandClientChange) SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	else SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
}

void Server::SetObjectFloat(int objectId, ObjectVariable k, float v, bool expandClientChange){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_FLOAT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	if(!expandClientChange) SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	else SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
}

void Server::SetObjectIntVec(int objectId, ObjectVariable k, vector3di v, bool expandClientChange){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_VECINT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	if(!expandClientChange) SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	else SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
}

void Server::SetObjectFloatVec(int objectId, ObjectVariable k, vector3df v, bool expandClientChange){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_VECFLOAT);
	stateChange.Write(objectId);
	stateChange.Write(k);
	stateChange.Write(v);
	if(!expandClientChange) SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	else SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
}