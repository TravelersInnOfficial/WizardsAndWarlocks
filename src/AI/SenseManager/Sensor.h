#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <AICodes.h>

class Signal;
class Modality;
class Blackboard;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

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