#include "Sensor.h"
#include "Modality.h"
#include "Signal.h"

Sensor::Sensor(int _id, vector3df* pos, vector3df* ori, float thresh, Blackboard* inf){
	id = _id;
	position = pos;
	orientation = ori;
	information = inf;
	threshold = thresh;
}

Sensor::~Sensor(){
	//La posicion y la Orientacion vienen del IAPlayer, ya se encargará ella de gestionarlos
}

bool Sensor::DetectsModality(Modality* mod){
	AI_modalities type = mod->GetType();
	if(type == AI_SIGHT){
		return true;
	}else if(type == AI_HEARING){
		return true;
	}
	return false;
}

void Sensor::Notify(Signal* sig, float currentTime){
	AI_modalities type = sig->GetModality()->GetType();

	bool temporal = !sig->GetTemporal(); // 0 - Se queda para siempre en memoria // 1 - Tiene un tiempo de memoria

	switch(type){
		case AI_SIGHT:
			information->SetSight(sig->GetId(), sig->GetPointer(), sig->GetCode(), sig->GetKinematic(), (currentTime + 2000)*temporal); // Tiempo en milisegundos
			break;
		case AI_HEARING:
			information->SetSound(sig->GetId(), sig->GetPointer(), sig->GetCode(), sig->GetKinematic(), (currentTime + 5000)*temporal); // Tiempo en milisengundos
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

int Sensor::GetId(){
	return id;
}