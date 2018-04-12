#ifndef REGIONALSENSEMANAGER_H
#define REGIONALSENSEMANAGER_H

#include <iostream>
#include <vector>
#include <kinematicTypes.h>

#include "Signal.h"
#include "Sensor.h"
#include "Modality.h"
#include "./../Blackboard.h"

class GraphicEngine;

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
	void InitObject();
	void EmptyObject();

	Sensor* AddSensor(int id, vector3df* cPos, vector3df* cOri, float thresh, Blackboard* inf);
	void AddSignal(int id, void* punt, bool temp, AI_code name, float str, Kinematic kin, AI_modalities mod);
	void AddSignal(Signal* s);
	void CreateAllSignals();
	void SendSignals();
	void ResetSensor(Sensor* s);
	void DeleteSensor(Sensor* s);

private:

	std::vector<Sensor*> sensors;
	std::vector<Notification*> notificationQueue;
	GraphicEngine* g_engine;

	RegionalSenseManager();
	RegionalSenseManager(RegionalSenseManager&);
	RegionalSenseManager operator =(RegionalSenseManager&);
};

#endif