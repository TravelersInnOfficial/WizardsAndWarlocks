#ifndef CONTROLOBJECT_H
#define CONTROLOBJECT_H

#include <iostream>
#include <vector>

#include "./Objects/Door.h"
#include "./Objects/Switch.h"
#include "./Objects/Potion.h"
#include "./Objects/Fountain.h"

class ControlObject{
public:
	static ControlObject* GetInstance();
	~ControlObject();
	Door* AddDoor();
	Switch* AddSwitch(Door* d);
	Potion* AddPotion(vector3df pos);
	Fountain* AddFountain();
	void DeletePotion(Potion* p);
	void Update();
private:
	void UpdateDoors();
	void UpdateSwitchs();
	void UpdatePotions();
	void UpdateFountains();

	vector<Door*>		doors;
	vector<Switch*>		switchs;
	vector<Potion*>		potions;
	vector<Fountain*>	fountains;


	ControlObject();
	static ControlObject* instance; 
};

#endif