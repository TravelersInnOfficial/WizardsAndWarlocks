#ifndef MANAGEROBJECT_H
#define MANAGEROBJECT_H

#include <iostream>
#include <vector>

#include "./Objects/Door.h"
#include "./Objects/Switch.h"
#include "./Objects/Potion.h"
#include "./Objects/Fountain.h"
#include "./Objects/Grail.h"

class ManagerObject{
public:
	static ManagerObject* GetInstance();
	~ManagerObject();
	Door* AddDoor();
	Grail* AddGrail();
	Switch* AddSwitch(Door* d);
	Potion* AddPotion(vector3df pos);
	Fountain* AddFountain();
	void DeletePotion(Potion* p);
	void Update();
private:
	void UpdateGrail();
	void UpdateDoors();
	void UpdateSwitchs();
	void UpdatePotions();
	void UpdateFountains();

	vector<Door*>		doors;
	vector<Switch*>		switchs;
	vector<Potion*>		potions;
	vector<Fountain*>	fountains;
	Grail*				grail;

	ManagerObject();
	static ManagerObject* instance; 
};

#endif