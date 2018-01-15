#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <AICodes.h>
#include <kinematicTypes.h>

class Modality;

class Signal{
public:
	Signal(Signal* sig);
	Signal(int id, void* punt, bool temp, AI_code name, float str, Kinematic kin, AI_modalities mod);
	~Signal();
	void* GetPointer();
	Kinematic GetKinematic();
	Modality* GetModality();
	float GetStrength();
	AI_code GetCode();
	int	GetId();
	vector3df GetPosition();
	bool GetTemporal();
private:
	int 		id;				// Id del objeto emisor de la señal
	void*		pointer;		// Puntero del objeto emisor de la señal
	bool 		temporal;		// Es una señal temporal o perdurará en la memoria? Si/No
	AI_code 	code;			// Codigo de IA que servirá para su posterior busqueda
	float 		strength;		// Fuerza de la señal
	Kinematic 	kinematic;		// Objeto kinematico de la señal
	Modality* 	modality;		// Modalidad de la senyal (Visual, Sonora)
};

#endif