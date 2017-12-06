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

class ObjectManager{
public:
	static ObjectManager* GetInstance();
	~ObjectManager();

	// Create Block and adds to vector
	Block* AddBlock(vector3df pos, vector3df size = vector3df(1,1,1), vector3df rot = vector3df(0,0,0), std::string texture=0);
	Door* AddDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Grail* AddGrail();
	Switch* AddSwitch(Door* d, vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Switch* AddSwitch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter);
	Potion* AddPotion(vector3df TPosition, vector3df TScale, vector3df TRotation);
	Fountain* AddFountain();
	void DeletePotion(Potion* p);
	void Update();
private:
	void UpdateGrail();
	void UpdateDoors();
	void UpdateSwitchs();
	void UpdatePotions();
	void UpdateFountains();
	void UpdateBlocks();

	vector<Block*>		blocks;
	vector<Door*>		doors;
	vector<Switch*>		switchs;
	vector<Potion*>		potions;
	vector<Fountain*>	fountains;
	Grail*				grail;

	ObjectManager();
	static ObjectManager* instance; 
};

#endif