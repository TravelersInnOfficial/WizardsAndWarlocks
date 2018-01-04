#include "Server.h"
#include "./../States/NetGame.h"
#include "./../Managers/PlayerManager.h"

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
	// SendShutdown();
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Server::SendShutdown(){
	peer->Shutdown(300);
}

void Server::SendPackage(RakNet::BitStream* bitstream, PacketPriority priority, PacketReliability reliability, RakNet::AddressOrGUID guid, bool broadcast){
	peer->Send(bitstream, priority, reliability, 0, guid, broadcast);
}

int Server::CreateNetworkObject(ObjectType type, bool broadcast, RakNet::RakNetGUID guid){
	lastObjectId += 1;
	networkObjects[lastObjectId] = new NetworkObject(lastObjectId, type);
	newNetworkObjects[lastObjectId] = networkObjects[lastObjectId];

	// We create the object on the server
	RakNet::BitStream bitstream;
	bitstream.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	bitstream.Write(ID_CREATE);
	bitstream.Write(lastObjectId);
	bitstream.Write(type);
	SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, guid, broadcast);

	return (lastObjectId);
}

void Server::RemoveNetworkObject(int id, bool broadcast){
	// We delete the object on the server
	if(broadcast){
		RakNet::BitStream bitstream;
		bitstream.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
		bitstream.Write(ID_REMOVE);
		bitstream.Write(id);
		SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	}

	// Eliminamos el local
	toEraseNetworkObjects[id] = networkObjects[id];
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

std::map<int, NetworkObject*> Server::GetToEraseNetworkObjects(){
	std::map<int, NetworkObject*> toRet = toEraseNetworkObjects;
	toEraseNetworkObjects.clear();
	return(toRet);
}

std::map<int, NetworkObject*> Server::GetNewNetworkObjects(){
	std::map<int, NetworkObject*> toRet = newNetworkObjects;
	newNetworkObjects.clear();
	return(toRet);
}

void Server::RecievePackages(){

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {

			// CUANDO SE CONECTA UN CLIENTE
			case ID_NEW_INCOMING_CONNECTION: {
				
				// Si la partida ha empezado negamos la conexion
				if(!NetGame::GetInstance()->GetLobbyState()){
					RakNet::BitStream bitstream;
					bitstream.Write((RakNet::MessageID)ID_DISCONNECTION_NOTIFICATION);
					SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					continue;
				}

				// Nos guardamos el nuevo cliente
				int id = AddPlayer(packet->guid);

				// Notificamos a todos los usuarios de que se ha conectado un player
				RakNet::BitStream bitstream;
				bitstream.Write((RakNet::MessageID)ID_PLAYER_JOIN);
				bitstream.Write(id);
				bitstream.Write(packet->guid);
				SendPackage(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);

				// Creamos el nuevo jugador y lo enviamos a todos los clientes
				// Menos al nuevo jugador, que se lo enviaremos mas tarde
				int newPlayerId = CreateNetworkObject(ID_PLAYER_O, true, packet->guid);
				
				// Guardamos la correspondencia entre cliente y jugador (GUID -- Player ID)
				clientToPlayer[id] = newPlayerId;
				
				// Le decimos al cliente nuevo cual es su PLAYER ONE
				RakNet::BitStream bitstream2;
				bitstream2.Write((RakNet::MessageID)ID_CREATE_PLAYER_ONE);
				bitstream2.Write(newPlayerId);
				SendPackage(&bitstream2, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);

				// Notificamos al nuevo jugador de todos los CLIENTES existentes hasta el momento
				// (En el cliente guardaremos su correspondencia GUID/ID DE PLAYER)
				for (auto &rowClient : networkPlayers) {
					if (rowClient.second != packet->guid){
						RakNet::BitStream updateClients;
						updateClients.Write((RakNet::MessageID)ID_EXISTING_PLAYER);
						updateClients.Write(rowClient.first);
						updateClients.Write(rowClient.second);
						SendPackage(&updateClients, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					}
				}

				// Notificamos al nuevo jugador de todos los OBJETOS DE RED existentes hasta el momento
				// Pasandole su ID, en el cliente replicaremos el Objeto de Red
				// Incluido su jugador, que se creara aqui pero para los demas se ha creado antes
				for (auto &rowObj : networkObjects) {
					RakNet::BitStream updateObjects;
					updateObjects.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
					updateObjects.Write(ID_EXISTING_OBJECT);
					updateObjects.Write(rowObj.first);
					updateObjects.Write(rowObj.second->GetObjType());
					SendPackage(&updateObjects, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
				}

				PlayerManager::GetInstance()->RefreshServerAll();

				break;
			}

			// CUANDO SE DESCONECTA UN CLIENTE
			case ID_CONNECTION_LOST: 
			case ID_DISCONNECTION_NOTIFICATION: {

				// Delete the player from the server
				int id = RemovePlayer(packet->guid);

				// Delete the player obj from the world
				std::map<int,int>::iterator i = clientToPlayer.find(id);
				if(i != clientToPlayer.end()){
					RakNet::BitStream deleteWorldPlayer;
					deleteWorldPlayer.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
					deleteWorldPlayer.Write(ID_REMOVE);
					deleteWorldPlayer.Write(i->second);
					RemoveNetworkObject(i->second);
					SendPackage(&deleteWorldPlayer, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
				}

				// Send the disconnect message to all users
				RakNet::BitStream disconnectClient;
				disconnectClient.Write((RakNet::MessageID)ID_PLAYER_DISCONNECT);
				disconnectClient.Write(id);
				SendPackage(&disconnectClient, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);

				break;
			}

			// CUANDO SE CREA/ELIMINA/MODIFICA UN OBJETO
			case ID_OBJECT_STATUS_CHAGED: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				ModifyObject(&bitstream);

				break;
			}

			// CUANDO SE TERMINA UNA PARTIDA
			case ID_MATCH_ENDED: {
				// Leer quien ha ganado
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				Alliance winnerAlliance;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(winnerAlliance);
				NetGame::GetInstance()->MatchEnded(winnerAlliance);

				// Propagarlo a los DEMAS clientes
				RakNet::BitStream propagateEndMatch;
				propagateEndMatch.Write((RakNet::MessageID)ID_MATCH_ENDED);
				propagateEndMatch.Write(winnerAlliance);
				SendPackage(&propagateEndMatch, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
				
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