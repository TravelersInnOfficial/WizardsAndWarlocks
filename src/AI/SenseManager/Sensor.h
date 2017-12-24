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
	Sensor(vector3df* pos, vector3df* ori, float thresh, Blackboard* inf);
	~Sensor();
	bool DetectsModality(Modality* mod);
	void Notify(Signal* sig, float currentTime);
	vector3df GetPosition();
	vector3df GetOrientation();
	float GetThreshold();
private:
	vector3df* position;
	vector3df* orientation;
	Blackboard* information;
	float threshold;
};

#endif