#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <vector3d.h>
#include <AICodes.h>
#include <kinematicTypes.h>

#include "./../Blackboard.h"

class Modality;
class Signal;

class Sensor{
public:
	Sensor(int id, vector3df* pos, vector3df* ori, float thresh, Blackboard* inf);
	~Sensor();
	bool DetectsModality(Modality* mod);
	void Notify(Signal* sig, float currentTime);
	Blackboard* GetBlackboard();
	vector3df GetPosition();
	vector3df GetOrientation();
	float GetThreshold();
	int GetId();
private:
	vector3df* 	m_position;
	vector3df* 	m_orientation;
	Blackboard* m_information;
	int 		m_id;
	float 		m_threshold;
};

#endif