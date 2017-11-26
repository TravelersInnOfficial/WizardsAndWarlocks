#ifndef CONTROLOBJECT_H
#define CONTROLOBJECT_H

#include <iostream>
#include <vector>

#include "./Objects/Door.h"
#include "./Objects/Switch.h"

class ControlObject{
public:
	static ControlObject* GetInstance();
	~ControlObject();
	Door* AddDoor();
	Switch* AddSwitch(Door* d);
	void Update();
private:
	void UpdateDoors();
	void UpdateSwitchs();

	vector<Door*>		doors;
	vector<Switch*>		switchs;


	ControlObject();
	static ControlObject* instance; 
};

#endif