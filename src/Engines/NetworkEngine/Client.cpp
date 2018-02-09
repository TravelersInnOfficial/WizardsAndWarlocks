#include "Client.h"
#include "Names.h"
#include "./../Managers/TrapManager.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/NetworkManager.h"
#include "./../Managers/StateManager.h"

Client::Client(std::string serverIp, int serverPort, bool proprietary){
	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1, &descriptor, 1);
	peer->SetTimeoutTime(15000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	peer->Connect(serverIp.c_str(), serverPort, 0, 0);
	playerOneId = -1;
	this->proprietary = proprietary;
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

void Client::EraseTrap(int trapId, int playerAffectedId){
	if(playerAffectedId != -1){
		Trap* trap = TrapManager::GetInstance()->GetTrapWithId(trapId);
		Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerAffectedId);
		if(trap != NULL && player != NULL) trap->ForceEffect(player);
	}
	else TrapManager::GetInstance()->IdErase(trapId);
}

void Client::CreateNetworkObject(int id, ObjectType type){
	networkObjects[id] = newNetworkObjects[id] = new NetworkObject(id, type);
}

void Client::RemoveNetworkObject(int id){
	if(networkObjects[id] != NULL){
		toEraseNetworkObjects[id] = networkObjects[id];
		networkObjects.erase(id);
	}
}

std::map<int, NetworkObject*> Client::GetNetworkObjects(){
	return(networkObjects);
}

std::map<int, NetworkObject*> Client::GetToEraseNetworkObjects(){
	std::map<int, NetworkObject*> toRet = toEraseNetworkObjects;
	toEraseNetworkObjects.clear();
	return(toRet);
}

std::map<int, NetworkObject*> Client::GetNewNetworkObjects(){
	std::map<int, NetworkObject*> toRet = newNetworkObjects;
	newNetworkObjects.clear();
	return(toRet);
}

void Client::SetClientName(std::string name){
	if(name.length() <= 0 || name.compare("Player Name") == 0){
		int arraySize = sizeof(defaultNames)/sizeof(defaultNames[0]);
		int index = rand() % arraySize;
		name = defaultNames[index];
	}

	if(name.length() > 15) name = name.substr(0,15);
	this->name = name;
}

std::string Client::GetClientName(){ return (name); }

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

			// NOS GUARDA LA ID DEL NETWORK OBJECT PLAYER ONE
			case ID_CREATE_PLAYER_ONE:{
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				
				// ID del NetworkObject PlayerOne, no ID del Jugador
				int id;
				bitstream.Read(id);
				playerOneId = id;
				if(proprietary) SendProprietary();

				break;
			}

			// CUANDO UN JUGADOR SE DESCONECTA
			case ID_PLAYER_DISCONNECT: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int id;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(id);
				break;
			}

			// CUANDO SE PIERDE LA CONEXION CON EL SERVER
			case ID_CONNECTION_ATTEMPT_FAILED:
			case ID_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY:
			case ID_OUR_SYSTEM_REQUIRES_SECURITY:
			case ID_PUBLIC_KEY_MISMATCH:
			case ID_ALREADY_CONNECTED:
			case ID_IP_RECENTLY_CONNECTED:
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			case ID_CONNECTION_BANNED:
			case ID_INVALID_PASSWORD:
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION: {
				StateManager::GetInstance()->PrepareStatus(STATE_MENU);
				break;
			}

			// CUANDO SE CREA/ELIMINA/MODIFICA UN OBJETO
			case ID_OBJECT_STATUS_CHAGED: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				ModifyObject(&bitstream);
				break;
			}

			// CUANDO SE ELIMINA UNA TRAMPA
			case ID_ERASE_TRAP: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int trapId = -1;
				int playerAffected = -1;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(trapId);
				bitstream.Read(playerAffected);
				EraseTrap(trapId, playerAffected);
				break;
			}

			// CUANDO UN PLAYER CAMBIA SUS TRAMPAS
			case ID_CHANGE_TRAP: {
				NetworkManager::GetInstance()->Update();
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int playerId = -1;
				TrapEnum trap = TENUM_NO_TRAP;
				int usings = -1;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(playerId);
				bitstream.Read(trap);
				bitstream.Read(usings);
				Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerId);
				if(player != NULL){
					TrapManager::GetInstance()->setPlayerTrap(player, trap, false);
					TrapManager::GetInstance()->setPlayerUsings(player, usings);
				}
				break;
			}

			// CUANDO UN PLAYER CAMBIA SUS HECHIZOS
			case ID_CHANGE_SPELL: {
				NetworkManager::GetInstance()->Update();
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int playerId = -1;
				int spellPosition = -1;
				SPELLCODE spell = NO_SPELL;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(playerId);
				bitstream.Read(spellPosition);
				bitstream.Read(spell);
				Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerId);
				if(player != NULL) SpellManager::GetInstance()->AddHechizo(spellPosition, player, spell, false);
				break;
			}

			// CUANDO SE INTERACCIONA CON UNA PUERTA
			case ID_DOOR_INTERACTED: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int doorVecPos = -1;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(doorVecPos);
				ObjectManager::GetInstance()->UseNetworkDoor(doorVecPos);
				break;
			}

			// CUANDO SE INTERACCIONA CON UNA POCION
			case ID_POTION_INTERACTED: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int potionVecPos = -1;
				int playerNetworkId = -1;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(potionVecPos);
				bitstream.Read(playerNetworkId);
				Player* player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerNetworkId);
				if(player != NULL) ObjectManager::GetInstance()->UseNetworkPotion(potionVecPos, player);
				break;
			}

			// CUANDO NOS CONECTAMOS Y QUEREMOS SINCRONIZAR UNA PUERTA
			case ID_DOOR_FORCE_OPEN: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				int doorVecPos = -1;
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(doorVecPos);
				std::vector<Door*> doors = ObjectManager::GetInstance()->GetAllDoors();
				Door* d = doors.at(doorVecPos);
				if(d != NULL) d->ForceOpen();
				break;
			}

			// CUANDO NOS CONECTAMOS Y QUEREMOS SINCRONIZAR UNA POCION
			case ID_REFRESH_POTION: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				
				int potionVecPos = -1;
				bool picked = false;
				int playerNetId = -1;
				vector3df pos = vector3df(0, 0, 0);
				
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(potionVecPos);
				bitstream.Read(picked);

				std::vector<Potion*> potions = ObjectManager::GetInstance()->GetAllPotions();
				Potion* p = potions.at(potionVecPos);

				if(p != NULL){
					if(picked){
						bitstream.Read(playerNetId);
						Player* player = NULL;
						player = PlayerManager::GetInstance()->GetPlayerFromNetID(playerNetId);
						if(player != NULL) p->NetInteract(player);
					}
					else{
						bitstream.Read(pos);
						p->SetPosition(pos);
					}
				}
				
				break;
			}
			
			// CUANDO NOS CONECTAMOS Y QUEREMOS SINCRONIZAR LAS TRAMPAS
			case ID_INIT_TRAPS: {
				RakNet::BitStream bitstream(packet->data, packet->length, false);
				vector3df pos = vector3df(0, 0, 0);
				vector3df normal = vector3df(0, 0, 0);
				int type = -1;
				int id = -1;
				
				bitstream.IgnoreBytes(sizeof(RakNet::MessageID));
				bitstream.Read(pos);
				bitstream.Read(normal);
				bitstream.Read(type);
				bitstream.Read(id);

				TrapManager::GetInstance()->NoPlayerDeploy(pos, normal, (TrapEnum)type, id);
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
		case ID_EXISTING_OBJECT:
		case ID_CREATE: {
			int k = -1;
			ObjectType o = ID_NO_OBJ;
			bitstream->Read(k);
			bitstream->Read(o);
			if(o == ID_TRAP_O) SetTrap(k, bitstream);
			else CreateNetworkObject(k, o);
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
			if(networkObjects[k] != NULL) networkObjects[k]->SetBoolVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_INT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			int v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL) networkObjects[k]->SetIntVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_FLOAT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			float v = -1;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL) networkObjects[k]->SetFloatVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_VECINT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3di v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL) networkObjects[k]->SetVecIVar(k_var, v, false, false);
			break;
		}
		case ID_CHANGE_VECFLOAT: {
			int k = -1;
			ObjectVariable k_var = ID_NO_VAR;
			vector3df v;
			bitstream->Read(k);
			bitstream->Read(k_var);
			bitstream->Read(v);
			if(networkObjects[k] != NULL) networkObjects[k]->SetVecFVar(k_var, v, false, false);
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
			if(networkObjects[k] != NULL) networkObjects[k]->SetStringVar(k_var, v, false, false);
			break;
		}
	}

}

void Client::SetTrap(int trapId, RakNet::BitStream* bitstream){
	vector3df position = vector3df(0, 0, 0);
	vector3df normal = vector3df(0, 0, 0);
	int playerId = -1;

	bitstream->Read(position);
	bitstream->Read(normal);
	bitstream->Read(playerId);

	TrapManager::GetInstance()->DirectDeploy(playerId, position, normal, trapId);
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

void Client::SetObjectString(int objectId, ObjectVariable k, std::string v){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_OBJECT_STATUS_CHAGED);
	stateChange.Write(ID_CHANGE_STRING);
	stateChange.Write(objectId);
	stateChange.Write(k);

	RakNet::RakString vAux = RakNet::RakString(v.c_str());
	stateChange.Write(vAux);

	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}

int Client::GetPlayerOneId(){ return (playerOneId); }

void Client::SetPlayerTrap(int networkId, TrapEnum trap){
	RakNet::BitStream newTrapsMessage;
	newTrapsMessage.Write((RakNet::MessageID)ID_CHANGE_TRAP);
	
	newTrapsMessage.Write(networkId);
	newTrapsMessage.Write(trap);

	SendPackage(&newTrapsMessage, HIGH_PRIORITY, RELIABLE_ORDERED);
}

void Client::SetPlayerSpell(int networkId, int spellPosition, SPELLCODE spell){
	RakNet::BitStream newSpellMessage;
	newSpellMessage.Write((RakNet::MessageID)ID_CHANGE_SPELL);
	
	newSpellMessage.Write(networkId);
	newSpellMessage.Write(spellPosition);
	newSpellMessage.Write(spell);

	SendPackage(&newSpellMessage, HIGH_PRIORITY, RELIABLE_ORDERED);
}

void Client::SendProprietary(){
	RakNet::BitStream stateChange;
	stateChange.Write((RakNet::MessageID)ID_IDENTIFY_PROPRIETARY);
	SendPackage(&stateChange, HIGH_PRIORITY, RELIABLE_ORDERED);
}