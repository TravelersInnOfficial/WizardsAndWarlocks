#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <AICodes.h>
#include <kinematicTypes.h>

class Modality;

class Signal{
public:
	Signal(Signal* sig);
	Signal(int id, AI_code name, float str, Kinematic kin, AI_modalities mod);
	~Signal();
	Kinematic GetKinematic();
	Modality* GetModality();
	float GetStrength();
	AI_code GetCode();
	int	GetId();
	vector3df GetPosition();
private:
	int id;
	AI_code code;
	float strength;
	Kinematic kinematic;
	Modality* modality;
};

#endif