#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <AICodes.h>
#include <kinematicTypes.h>

class Modality;

class Signal{
public:
	Signal(Signal* sig);
	Signal(int id, AI_code name, void* emi, float str, Kinematic kin);
	~Signal();
	Kinematic GetKinematic();
	Modality* GetModality();
	float GetStrength();
	AI_code GetCode();
	int	GetId();
	void* GetEmisor();
	vector3df GetPosition();
private:
	int id;
	AI_code code;
	void* emisor;
	float strength;
	Kinematic kinematic;
	Modality* modality;
};

#endif