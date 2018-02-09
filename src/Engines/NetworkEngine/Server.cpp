#include "Server.h"
#include "./../Managers/PlayerManager.h"
#include "./../Players/Player.h"

#include "./../Managers/TrapManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/ObjectManager.h"
#include "./../Managers/StateManager.h"

Server::Server(int serverPort, int maxClients, bool createdFromGame){
	this->createdFromGame = createdFromGame;

	peer = RakNet::RakPeerInterface::GetInstance();
	descriptor = RakNet::SocketDescriptor(serverPort, 0);
	peer->Startup(maxClients, &descriptor, 1);
	peer->SetMaximumIncomingConnections(maxClients);

	// Creo el NetworkObject del juego multijugador
	lastObjectId += 1;
	multiGameId = lastObjectId;
	multiGameObject = new NetworkObject(lastObjectId, ID_MULTIGAME_O);
	networkObjects[lastObjectId] = multiGameObject;
	newNetworkObjects[lastObjectId] = networkObjects[lastObjectId];
	
	// Inicializo su variable
	multiGameObject->SetBoolVar(MULTIGAME_CHANGE, false, true, false);
	multiGameObject->SetIntVar(MULTIGAME_WINNER_ALLIANCE, (int)NO_ALLIANCE, true, false);

	maxTimeToConnectPlayerOne = 15;
	playerOneID = RakNet::UNASSIGNED_RAKNET_GUID;
}

Server::~Server(){
	// Delete all network objects
	std::map<int, NetworkObject*>::iterator i = networkObjects.begin();
	for(;i!=networkObjects.end(); i++) delete i->second;

	// Destroy the PEER interface
	SendShutdown();
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void Server::Update(float deltaTime){
	if(createdFromGame) CheckIfPlayerOneConnected(deltaTime);
}

void Server::CheckIfPlayerOneConnected(float deltaTime){
	if(playerOneID == RakNet::UNASSIGNED_RAKNET_GUID){
		maxTimeToConnectPlayerOne -= deltaTime;
		if(maxTimeToConnectPlayerOne <= 0) StateManager::GetInstance()->CloseGame();
	}
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

// REMEMBER: IS THE NETWORK ID, NOT THE ENTITY ID
void Server::SetTrap(vector3df point,vector3df normal, int playerId, int trapId){
	RakNet::BitStream setTrapMessage;
	
	setTrapMessage.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);	// Message ID
	setTrapMessage.Write(ID_CREATE);	// Message Sub ID
	
	setTrapMessage.Write(trapId);		// K
	setTrapMessage.Write(ID_TRAP_O);	// O

	setTrapMessage.Write(point);		//|
	setTrapMessage.Write(normal);		//| 
	setTrapMessage.Write(playerId);		//| ETC 
	
	SendPackage(&setTrapMessage, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::EraseTrap(int trapId, int playerAffectedId){
	RakNet::BitStream eraseTrapMessage;
	eraseTrapMessage.Write((RakNet::MessageID)ID_ERASE_TRAP);
	eraseTrapMessage.Write(trapId);
	eraseTrapMessage.Write(playerAffectedId);
	SendPackage(&eraseTrapMessage, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
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

void Server::RecievePackages(bool isLobby){

	// Set PONG data
	SetServerData(isLobby);

	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) {
		switch (packet->data[0]) {

			// CUANDO SE CONECTA UN CLIENTE
			case ID_NEW_INCOMING_CONNECTION: {

				// Si la partida ha empezado negamos la conexion
				if(!isLobby){
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
				TrapManager::GetInstance()->RefreshServerAll();
				SpellManager::GetInstance()->RefreshServerAll();

				// SYNC DOORS AT THE BEGINING (Open State)
				std::vector<Door*> doors = ObjectManager::GetInstance()->GetAllDoors();
				for(int i = 0; i < doors.size(); i++){
					Door* d = doors.at(i);
					if(d != NULL && d->GetOpenState()){
						RakNet::BitStream updateDoors;
						updateDoors.Write((RakNet::MessageID)ID_DOOR_FORCE_OPEN);
						updateDoors.Write(i);
						SendPackage(&updateDoors, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					}
				}

				// SYNC POTIONS AT THE BEGINING (Owner && Pos)
				std::vector<Potion*> potions = ObjectManager::GetInstance()->GetAllPotions();
				for(int i = 0; i < potions.size(); i++){
					Potion* p = potions.at(i);
					if(p != NULL){
						RakNet::BitStream updatePotions;
						updatePotions.Write((RakNet::MessageID)ID_REFRESH_POTION);
						updatePotions.Write(i);
						
						bool isPicked = false;
						isPicked = p->GetPickedState();
						updatePotions.Write(isPicked);

						if(isPicked){
							Player* byWho = NULL;
							byWho = p->GetUser();
							if(byWho != NULL){
								NetworkObject* nObj = byWho->GetNetworkObject();
								if(nObj != NULL){
									int nObjId = nObj->GetObjId();
									updatePotions.Write(nObjId);
								}
							}
						}
						else{
							vector3df pos = vector3df(0, 0, 0);
							pos = p->GetPosition();
							updatePotions.Write(pos);
						}

						SendPackage(&updatePotions, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					}
				}

				// SYNC TRAPS AT THE BEGINING (Pos && Type)
				std::vector<Trap*> traps = TrapManager::GetInstance()->GetAllTraps();
				for(int i = 0; i < traps.size(); i++){
					Trap* t = traps.at(i);
					if(t != NULL){
						RakNet::BitStream updateTraps;
						vector3df pos = t->GetPosition();
						vector3df normal = t->GetNormal();
						int type = (int) t->GetTrapType();
						int id = t->GetTrapId();

						updateTraps.Write((RakNet::MessageID)ID_INIT_TRAPS);
						updateTraps.Write(pos);
						updateTraps.Write(normal);
						updateTraps.Write(type);
						updateTraps.Write(id);

						SendPackage(&updateTraps, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, false);
					}
				}

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

				if(createdFromGame && playerOneID == packet->guid) StateManager::GetInstance()->CloseGame();
				break;
			}

			// CUANDO SE CREA/ELIMINA/MODIFICA UN OBJETO
			case ID_OBJECT_STATUS_CHAGED: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				ModifyObject(&bitstream);

				break;
			}

			// CUANDO SE TERMINA UNA PARTIDA
			case ID_IDENTIFY_PROPRIETARY: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				if(createdFromGame && playerOneID == RakNet::UNASSIGNED_RAKNET_GUID) playerOneID = packet->guid;
				break;
			}

			// CUANDO UN PLAYER CAMBIA SUS TRAMPAS
			case ID_CHANGE_TRAP: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int playerId = -1;
				TrapEnum trap = TENUM_NO_TRAP;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(playerId);
				bitstream.Read(trap);
				Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerId);
				if(player != NULL){
					TrapManager::GetInstance()->setPlayerTrap(player, trap);
					
					RakNet::BitStream newTrapsMessage;
					newTrapsMessage.Write((RakNet::MessageID)ID_CHANGE_TRAP);
					newTrapsMessage.Write(playerId);
					newTrapsMessage.Write(trap);
					newTrapsMessage.Write(TrapManager::GetInstance()->getPlayerUsings(player));
					SendPackage(&newTrapsMessage, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
				}
				break;
			}

			// CUANDO UN PLAYER CAMBIA SUS HECHIZOS
			case ID_CHANGE_SPELL: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int playerId = -1;
				int spellPosition = -1;
				SPELLCODE spell = NO_SPELL;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(playerId);
				bitstream.Read(spellPosition);
				bitstream.Read(spell);
				Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerId);
				if(player != NULL){
					SpellManager::GetInstance()->AddHechizo(spellPosition, player, spell);

					RakNet::BitStream newSpellMessage;
					newSpellMessage.Write((RakNet::MessageID)ID_CHANGE_SPELL);
					newSpellMessage.Write(playerId);
					newSpellMessage.Write(spellPosition);
					newSpellMessage.Write(spell);
					SendPackage(&newSpellMessage, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
				}
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
		case ID_CHANGE_STRING: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			RakNet::RakString vAux = "";
			std::string v = "";
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(vAux);
			v = vAux;
			networkObjects[k]->SetStringVar(k_var, v, true, true);
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

void Server::SetObjectString(int objectId, ObjectVariable k, std::string v, bool expandClientChange){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_STRING);
	stateChange.Write(objectId);
	stateChange.Write(k);
	
	RakNet::RakString vAux = RakNet::RakString(v.c_str());
	stateChange.Write(vAux);

	if(!expandClientChange) SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
	else SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED, packet->guid, true);
}

void Server::EndMatch(Alliance winnerAlliance){
	RakNet::BitStream propagateEndMatch;
	propagateEndMatch.Write((RakNet::MessageID)ID_MATCH_ENDED);
	propagateEndMatch.Write(winnerAlliance);
	SendPackage(&propagateEndMatch, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::SetPlayerTrap(int networkId, TrapEnum trap, int usings){
	RakNet::BitStream newTrapsMessage;
	newTrapsMessage.Write((RakNet::MessageID)ID_CHANGE_TRAP);
	newTrapsMessage.Write(networkId);
	newTrapsMessage.Write(trap);
	newTrapsMessage.Write(usings);
	SendPackage(&newTrapsMessage, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::SetPlayerSpell(int networkId, int spellPosition, SPELLCODE spell){
	RakNet::BitStream newSpellMessage;
	newSpellMessage.Write((RakNet::MessageID)ID_CHANGE_SPELL);
	newSpellMessage.Write(networkId);
	newSpellMessage.Write(spellPosition);
	newSpellMessage.Write(spell);
	SendPackage(&newSpellMessage, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::NotifyDoorInteracted(int doorPos){
	RakNet::BitStream doorInteractMessage;
	doorInteractMessage.Write((RakNet::MessageID)ID_DOOR_INTERACTED);
	doorInteractMessage.Write(doorPos);
	SendPackage(&doorInteractMessage, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::NotifyPotionInteracted(int potionPos, Player* p){
	RakNet::BitStream potionInteractMessage;
	potionInteractMessage.Write((RakNet::MessageID)ID_POTION_INTERACTED);
	potionInteractMessage.Write(potionPos);
	if(p != NULL){
		NetworkObject* nObj = p->GetNetworkObject();
		if(nObj != NULL){
			potionInteractMessage.Write(p->GetNetworkObject()->GetObjId());
			SendPackage(&potionInteractMessage, HIGH_PRIORITY, RELIABLE_ORDERED, RakNet::UNASSIGNED_RAKNET_GUID, true);
		}
	}
}

bool Server::GetCreatedFromGame(){
	return createdFromGame;
}

void Server::SetServerData(bool isLobby){
	std::string name = "LobbyOne";
	
	std::string stringToSend = "";
	stringToSend =  std::to_string(networkPlayers.size());
	stringToSend += std::to_string(isLobby);
	
	int nameSize = stringToSend.length();
	std::string nameSize_s = std::to_string(nameSize);
	if(nameSize < 10) nameSize_s = "0" + nameSize_s;
	stringToSend += nameSize_s;

	stringToSend += name;
	
	char const* pchar = stringToSend.c_str();
	int pcharSize = (int) strlen(pchar) + 1;
	
	peer->SetOfflinePingResponse(pchar, pcharSize);
	std::cout << "Bits to write: " << pcharSize << std::endl;
}