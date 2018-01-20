#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <iostream>
#include <vector>

#include "./../Invocations/AllInvocations.h"
#include "./../DamageAreas/AllDamageAreas.h"
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
#include "./../Npcs/Npc.h"

#include <InvocationCodes.h>
#include <PotionTypes.h>
#include <AreaCodes.h>
#include <NPCTypes.h>
#include <Alliance.h>

class ObjectManager{
public:
	static ObjectManager* GetInstance();
	~ObjectManager();

	// Adders
	Block* AddBlock(vector3df pos, vector3df size = vector3df(1,1,1), vector3df rot = vector3df(0,0,0), std::string texture="");
	Prop* AddProp(vector3df pos, vector3df size, vector3df rot, std::string model, std::string texture);
	Door* AddDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Grail* AddGrail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Switch* AddSwitch(Door* d, vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Switch* AddSwitch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Potion* AddPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, POTIONTYPE type);
	Fountain* AddFountain(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Npc* AddNpc(vector3df TPosition, vector3df TScale, vector3df TRotation, NPCType type);
	Invocation* AddInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation, InvoEnum type);
	DamageArea* AddDamageArea(vector3df TPosition, vector3df TScale, vector3df TRotation, AreaEnum type);
	void AddSpawner(Alliance playerAlliance, vector3df TPosition);
	void AddReadyPoint(vector3df TPosition);
	void AddNavmesh(NavMesh);

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
	NavMesh GetNavMesh();

	// Drawers
	void DrawNpcMenu();
	void DrawGrailGUI();

	// Deleters
	void DeletePotion(Potion* p);
	void DeleteBlock(Block* b);
	void StopInteractionsNPC();

	// Actions
	void ClearMap();
	bool CheckIfWon();
	void Update(float deltaTime);

	// Door Sync
	int GetDoorVecPos(Door* door);
	int GetPotionVecPos(Potion* potion);
	void UseNetworkDoor(int doorVecPos);
	void UseNetworkPotion(int potionVecPos, Player* p);

private:

	// Updates
	void UpdateGrail(float deltaTime);
	void UpdateDoors();
	void UpdateSwitchs();
	void UpdatePotions();
	void UpdateFountains(float deltaTime);
	void UpdateBlocks();
	void UpdateNpcs();
	void UpdateInvocations(float deltaTime);
	void UpdateDamageAreas(float deltaTime);


	vector<Invocation*>			invocations;	// Vector donde se almacenan todas las invocaciones (creacion hechizo)
	vector<DamageArea*>			damageAreas;	// Vector donde se almacenan todas las areas de danyo	vector<Block*>
	vector<Block*>				blocks;			// Vector donde se almacenan todos los bloques (paredes, suelo)
	vector<Prop*>				props;			// Vector donde se almacenan todos los props (decoracion)
	vector<Door*>				doors;			// Vector donde se almacenan todas las puertas
	vector<Switch*>				switchs;		// Vector donde se almacenan todos los interruptores
	vector<Potion*>				potions;		// Vector donde se almacenan todas las pociones
	vector<Fountain*>			fountains;		// Vector donde se almacenan todas las fuentes
	vector<Npc*>				npcs;			// Vector donde se almacenan todos los npcs
	vector<vector3df>			wizardSpawn;	// Vector donde se almacenan todos los spawners de MAGOS
	vector<vector3df>			warlockSpawn;	// Vector donde se almacenan todos los spawners de BRUJOS
	Grail*						grail;			// El grail de la partida, unico
	vector4df					readyZone;		// Zona de READY
	NavMesh						navmesh;		// Malla de movimiento para la IA del nivel, única
	ObjectManager();
	static ObjectManager* instance; 
};

#endif