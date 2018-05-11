#include "Sensor.h"

#include "Modality.h"
#include "Signal.h"
#include "./../Blackboard.h"

Sensor::Sensor(int _id, vector3df* pos, vector3df* ori, float thresh, Blackboard* inf){
	m_id = _id;
	m_position = pos;
	m_orientation = ori;
	m_information = inf;
	m_threshold = thresh;
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
			m_information->SetSight(sig->GetId(), sig->GetPointer(), sig->GetCode(), sig->GetKinematic(), (currentTime + 2000)*temporal); // Tiempo en milisegundos
			break;
		case AI_HEARING:
			m_information->SetSound(sig->GetId(), sig->GetPointer(), sig->GetCode(), sig->GetKinematic(), (currentTime + 5000)*temporal); // Tiempo en milisengundos
			break;
	}
}

Blackboard* Sensor::GetBlackboard(){
	return m_information;
}

vector3df Sensor::GetPosition(){
	vector3df output(m_position->X, m_position->Y, m_position->Z);
	return output;
}

vector3df Sensor::GetOrientation(){
	vector3df output(m_orientation->X, m_orientation->Y, m_orientation->Z);
	return output;
}

float Sensor::GetThreshold(){
	return m_threshold;
}

int Sensor::GetId(){
	return m_id;
}