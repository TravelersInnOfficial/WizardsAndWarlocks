#include "Modality.h"
#include "Signal.h"
#include "Sensor.h"
#include <PhysicsEngine/BulletEngine.h>
#include "./../../Entidad.h"
#include "./../../Players/Player.h"

Modality::Modality(Modality* mod){
	maximumRange = mod->GetMaximumRange();
	attenuation = mod->GetAttenuation();
	inverseTransmissionSpeed = mod->GetInverseSpeed();
	type = mod->GetType();
}

Modality::Modality(float maxR, float att, float inSpeed){
	maximumRange = maxR;
	attenuation = att;
	inverseTransmissionSpeed = inSpeed;
}

Modality::~Modality(){}

bool Modality::ExtraChecks(Signal* sig, Sensor* sr){return true;}

float Modality::GetMaximumRange(){
	return maximumRange;
}

float Modality::GetAttenuation(){
	return attenuation;
}

float Modality::GetInverseSpeed(){
	return inverseTransmissionSpeed;
}

AI_modalities Modality::GetType(){
	return type;
}

// ===================================================================================================== //
//
//	SIGHT
//
// ===================================================================================================== //

SightModality::SightModality(Modality* mod):Modality(mod){}

SightModality::SightModality(float maxR, float att, float inSpeed):Modality(maxR, att, inSpeed){
	type = AI_SIGHT;
}

SightModality::~SightModality(){}

bool SightModality::ExtraChecks(Signal* sig, Sensor* sr){
	vector3df signalPos = sig->GetPosition();
	vector3df sensorPos = sr->GetPosition();
	vector3df sensorOri = sr->GetOrientation();
	//Hacer Comprobacion del Cono de Vision
	if(!checkSightCone(signalPos, sensorPos, sensorOri)){
		return false;
	}
	//Hacer comprobacion de la linea recta
	if(!checkLineOfSight(signalPos, sensorPos)){
		return false;
	}
	return true;
}

bool SightModality::checkSightCone(vector3df sigPos, vector3df srPos, vector3df srOri){
	vector3df dir = sigPos - srPos;

	float t = atan(dir.Y/(sqrt(dir.X*dir.X + dir.Z*dir.Z)));
	float p = atan2(dir.X,dir.Z);

	// ------------------------------------------------------------- CHECK Y AXIS
	float offset = p - srOri.Y;
	// Corregir el angulo del offset para que este [-PI, PI]
	if(offset>M_PI){
		offset -= 2*M_PI;
	}else if(offset<-M_PI){
		offset += 2*M_PI;
	}
	// Valor absoluto para poder contar todos los valores [-angulo, angulo]
	offset = abs(offset);
	// Comprobacion con el angulo en radianes
	if(!(offset<0.5235)){
		return false;
	} 
	// ------------------------------------------------------------ CHECK X AXIS
	offset = t - (-srOri.X);
	offset = abs(offset);
	if(!(offset<0.5235)){
		return false;
	}
	return true;
}

bool SightModality::checkLineOfSight(vector3df sigPos, vector3df srPos){
	BulletEngine* f_engine = BulletEngine::GetInstance();
	void* obj = f_engine->Raycast(sigPos, srPos);
	if(obj!=nullptr){
		Entidad* e = (Entidad*)obj;
		EntityEnum valor = e->GetClase();
		if(valor == EENUM_PLAYER){
			Player* p = (Player*)obj;
			vector3df pos = p->GetPos();
			if(pos == srPos){
				return true;
			}
		}
	}
	return false;
}

// ===================================================================================================== //
//
//	HEARING
//
// ===================================================================================================== //

HearingModality::HearingModality(Modality* mod):Modality(mod){}

HearingModality::HearingModality(float maxR, float att, float inSpeed):Modality(maxR, att, inSpeed){
	type = AI_HEARING;
}

HearingModality::~HearingModality(){}

bool HearingModality::ExtraChecks(Signal* sig, Sensor* sr){
	return true;
}
