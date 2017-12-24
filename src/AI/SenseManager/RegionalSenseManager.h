#ifndef REGIONALSENSEMANAGER_H
#define REGIONALSENSEMANAGER_H

#include <iostream>
#include <vector>
#include <kinematicTypes.h>

#include "Signal.h"
#include "Sensor.h"
#include "Modality.h"
#include "./../Blackboard.h"

struct Notification{
	float time;
	Sensor* sensor;
	Signal* signal;
	~Notification(){
		delete signal;
	}
};

class RegionalSenseManager{
public:
	static RegionalSenseManager* GetInstance();

	~RegionalSenseManager();
	Sensor* AddSensor(vector3df* cPos, vector3df* cOri, float thresh, Blackboard* inf);
	void AddSignal(int id, AI_code name, void* emisor, float str, Kinematic kin);
	void AddSignal(Signal* s);
	void SendSignals();
private:

	std::vector<Sensor*> sensors;
	std::vector<Notification*> notificationQueue;

	RegionalSenseManager();
	static RegionalSenseManager* instance;
};

#endif