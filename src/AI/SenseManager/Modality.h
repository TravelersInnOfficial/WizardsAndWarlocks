#ifndef MODALITY_H
#define MODALITY_H

#include <iostream>
#include <AICodes.h>


class Signal;
class Sensor;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class Modality{
public:
	Modality(Modality* mod);
	Modality(float maxR, float att, float inSpeed);
	virtual ~Modality();
	virtual bool ExtraChecks(Signal* sig, Sensor* sr);

	float GetMaximumRange();
	float GetAttenuation();
	float GetInverseSpeed();
	AI_modalities GetType();
protected:
	float maximumRange;
	float attenuation;
	float inverseTransmissionSpeed;

	AI_modalities type;
};

class SightModality: public Modality{
public:
	SightModality(Modality* mod);
	SightModality(float maxR, float att, float inSpeed);
	~SightModality();
	bool ExtraChecks(Signal* sig, Sensor* sr);
private:
	bool checkSightCone(vector3df sigPos, vector3df senPos, vector3df senOri);
	bool checkLineOfSight(vector3df sigPos, vector3df senPos);
};

class HearingModality: public Modality{
public:
	HearingModality(Modality* mod);
	HearingModality(float maxR, float att, float inSpeed);
	~HearingModality();
	bool ExtraChecks(Signal* sig, Sensor* sr);
private:
};

#endif