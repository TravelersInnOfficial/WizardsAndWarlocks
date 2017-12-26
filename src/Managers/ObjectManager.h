#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <iostream>
#include <vector>

#include "./../Objects/Block.h"
#include "./../Objects/Door.h"
#include "./../Objects/Switch.h"
#include "./../Objects/Potion.h"
#include "./../Objects/Fountain.h"
#include "./../Objects/Grail.h"
#include "./../Npcs/Npc.h"
#include "./../Npcs/NpcSelector.h"
#include "./../Objects/Invocation.h"
#include "./../Objects/DamageArea.h"
#include <NPCTypes.h>
#include <PotionTypes.h>

class ObjectManager{
public:
	static ObjectManager* GetInstance();
	~ObjectManager();

	// Adders
	Block* AddBlock(vector3df pos, vector3df size = vector3df(1,1,1), vector3df rot = vector3df(0,0,0), std::string texture=0);
	Door* AddDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Grail* AddGrail(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Switch* AddSwitch(Door* d, vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Switch* AddSwitch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Potion* AddPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, POTIONTYPE type);
	Fountain* AddFountain(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Npc* AddNpc(vector3df TPosition, vector3df TScale, vector3df TRotation, NPCType type);
	Invocation* AddInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);
	DamageArea* AddDamageArea(vector3df TPosition, vector3df TScale, vector3df TRotation);

	// Drawers
	void DrawNpcMenu();

	// Deleters
	void DeletePotion(Potion* p);

	// Actions
	void ClearMap();
	void Update(float deltaTime);

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
	vector<DamageArea*>			damageAreas;	// Vector donde se almacenan todas las areas de danyo
	vector<Block*>				blocks;			// Vector donde se almacenan todos los bloques (paredes, suelo)
	vector<Door*>				doors;			// Vector donde se almacenan todas las puertas
	vector<Switch*>				switchs;		// Vector donde se almacenan todos los interruptores
	vector<Potion*>				potions;		// Vector donde se almacenan todas las pociones
	vector<Fountain*>			fountains;		// Vector donde se almacenan todas las fuentes
	vector<Npc*>				npcs;			// Vector donde se almacenan todos los npcs
	Grail*						grail;			// El grail de la partida, unico

	ObjectManager();
	static ObjectManager* instance; 
};

#endif