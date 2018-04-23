#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <iostream>
#include <vector>

#include "./../Invocations/AllInvocations.h"
#include "./../DamageAreas/AllDamageAreas.h"
#include "./../AI/RoomGraph/RoomGraph.h"
#include "./../Objects/Fountain.h"
#include "./../Npcs/NpcSelector.h"
#include "./../Objects/NavMesh.h"
#include "./../Objects/Switch.h"
#include "./../Objects/Potion.h"
#include "./../Npcs/NpcSeller.h"
#include "./../Objects/Block.h"
#include "./../Objects/Prop.h"
#include "./../Objects/Grail.h"
#include "./../Objects/Door.h"
#include "./../Objects/PlayerParts.h"
#include "./../Npcs/Npc.h"

#include <InvocationCodes.h>
#include <PotionTypes.h>
#include <AreaCodes.h>
#include <NPCTypes.h>
#include <Alliance.h>
#include <GraphicEngine/Light.h>

class ObjectManager{
public:
	static ObjectManager* GetInstance();
	~ObjectManager();
	void InitObject();
	void EmptyObject();

	// Adders
	Block* 			AddBlock(vector3df pos, vector3df size = vector3df(1,1,1), vector3df rot = vector3df(0,0,0), std::string texture="");
	Prop* 			AddProp(vector3df pos, vector3df size, vector3df rot, std::string model, std::string texture);
	Door* 			AddDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter, std::string model3d);
	Grail* 			AddGrail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Switch* 		AddSwitch(Door* d, vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Switch* 		AddSwitch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Potion* 		AddPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, POTIONTYPE type);
	Fountain* 		AddFountain(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Npc* 			AddNpc(vector3df TPosition, vector3df TScale, vector3df TRotation, NPCType type);
	Invocation* 	AddInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation, InvoEnum type);
	DamageArea* 	AddDamageArea(vector3df TPosition, vector3df TScale, vector3df TRotation, AreaEnum type);
	void 			AddSpawner(Alliance playerAlliance, vector3df TPosition);
	void 			AddReadyPoint(vector3df TPosition);
	void 			AddNavmesh(std::string path);
	void 			AddRoomGraph(std::string path);
	PlayerParts*	AddPlayerParts(Alliance alliance, vector3df TPosition, vector3df TScale, vector3df TRotation);
	Light*			AddLight(vector3df TPosition, vector3df Color, float range);

	// SenseFunctions
	void SendAllSignal();
	void SendGrailSignal();
	void SendSwitchSignal();
	void SendPotionSignal();
	void SendFountainSignal();
	void SendInvocationSignal();

	// Getters
	vector3df GetRandomSpawnPoint(Alliance playerAlliance);
	vector4df GetReadyZone();
	NavMesh* GetNavMesh();
	std::vector<Door*> 	GetAllDoors();
	std::vector<Potion*> GetAllPotions();
	void CopyRoomGraph(RoomGraph* copy);

	// Setters
	void SetWizardSpawn(int num);
	void SetWarlockSpawn(int num);

	// Deleters
	void DeletePotion(Potion* p);
	void DeleteBlock(Block* b);
	void DeleteLight(Light* l);
	void StopInteractionsNPC();

	// Actions
	void ClearMap();
	bool CheckIfWon();
	void Update(float deltaTime);
	void SetWizardSpawn();
	void SetWarlockSpawn();

	// Door Sync
	int GetDoorVecPos(Door* door);
	int GetPotionVecPos(Potion* potion);
	void UseNetworkDoor(int doorVecPos);
	void UseNetworkPotion(int potionVecPos, Player* p);

private:
	void ReadJSONPotionsData();
	std::map<std::string, POTIONTYPE> GetPOTIONTYPE_StrMap();

	// Updates
	void UpdateGrail(float deltaTime);
	void UpdateDoors();
	void UpdateSwitchs();
	void UpdatePotions();
	void UpdateFountains(float deltaTime);
	void UpdateBlocks();
	void UpdateInvocations(float deltaTime);
	void UpdateDamageAreas(float deltaTime);
	void UpdatePlayerParts(float deltaTime);

	std::vector<Invocation*>	invocations;	// Vector donde se almacenan todas las invocaciones (creacion hechizo)
	std::vector<DamageArea*>	damageAreas;	// Vector donde se almacenan todas las areas de danyo	vector<Block*>
	std::vector<Light*>			lights;			// Vector donde se almacenan todas LAS LUCES
	std::vector<Block*>			blocks;			// Vector donde se almacenan todos los bloques (paredes, suelo)
	std::vector<Prop*>			props;			// Vector donde se almacenan todos los props (decoracion)
	std::vector<Door*>			doors;			// Vector donde se almacenan todas las puertas
	std::vector<Switch*>		switchs;		// Vector donde se almacenan todos los interruptores
	std::vector<Potion*>		potions;		// Vector donde se almacenan todas las pociones
	std::vector<Fountain*>		fountains;		// Vector donde se almacenan todas las fuentes
	std::vector<Npc*>			npcs;			// Vector donde se almacenan todos los npcs
	std::vector<PlayerParts*>	playerParts;	// Vector donde se almacenan los body parts al morir
	
	std::vector<vector3df>		wizardSpawn;	// Vector donde se almacenan todos los spawners de MAGOS
	std::vector<vector3df>		warlockSpawn;	// Vector donde se almacenan todos los spawners de BRUJOS
	int 						wizardSpawnSelected;	// Posicion del spawner seleccionado
	int 						warlockSpawnSelected;	// Posicion del spawner seleccionado

	Grail*						grail;			// El grail de la partida, unico
	vector4df					readyZone;		// Zona de READY
	NavMesh*					navmesh;		// Malla de movimiento para la IA del nivel, única
	RoomGraph*					roomGraph;		// Grafo de habitaciones del nivel, utilizado por la IA

	std::map<POTIONTYPE, std::string> potion_EFFECT;
	std::map<POTIONTYPE, std::string> potion_name;
	std::map<POTIONTYPE, std::string> potion_description;
	std::map<POTIONTYPE, float> potion_value;

	ObjectManager();
	ObjectManager(ObjectManager&);
	ObjectManager operator =(ObjectManager&);
};

#endif