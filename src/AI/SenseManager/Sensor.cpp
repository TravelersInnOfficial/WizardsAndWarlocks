#include "Sensor.h"
#include "Modality.h"
#include "Signal.h"

Sensor::Sensor(vector3df* pos, vector3df* ori, float thresh, Blackboard* inf){
	position = pos;
	orientation = ori;
	information = inf;
	threshold = thresh;
}

Sensor::~Sensor(){
	//La posicion y la Orientacion vienen del IAPlayer, ya se encargará ella de gestionarlos
}

bool Sensor::DetectsModality(Modality* mod){
	int type = mod->GetType();
	if(type == 0){
		return true;
	}
	return false;
}

void Sensor::Notify(Signal* sig, float currentTime){
	int tipo = sig->GetModality()->GetType();

	vector3df dir = sig->GetPosition() - GetPosition();
	float len = dir.length();

	switch(tipo){
		case 0:
			information->SetSight(sig->GetId(), sig->GetCode(), sig->GetEmisor(), sig->GetKinematic(), len, currentTime + 10000); //Tiempo en milisegundos
			break;
		case 1:
			break;
	}
}

vector3df Sensor::GetPosition(){
	vector3df output(position->X, position->Y, position->Z);
	return output;
}

vector3df Sensor::GetOrientation(){
	vector3df output(orientation->X, orientation->Y, orientation->Z);
	return output;
}

float Sensor::GetThreshold(){
	return threshold;
}