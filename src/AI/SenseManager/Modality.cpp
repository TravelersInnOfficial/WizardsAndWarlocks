#include "Modality.h"

#include <PhysicsEngine/BulletEngine.h>
#include "./../RoomGraph/RoomGraph.h"
#include "./../../Players/Player.h"
#include "./../../Entidad.h"
#include "./../Blackboard.h"
#include <Constants.h>
#include "Signal.h"
#include "Sensor.h"
#include <cmath>

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
	offset = std::abs(offset);
	// Comprobacion con el angulo en radianes
	if(!(offset<0.5235)){
		return false;
	} 
	// ------------------------------------------------------------ CHECK X AXIS
	offset = t - (-srOri.X);
	offset = std::abs(offset);
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
			vector3df pos = p->GetHeadPos();
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
	bool output = false;

	// Tenemos que comprobar que el sonido provenga de la misma habitacion o en todo caso de una contigua
	Blackboard* info = sr->GetBlackboard();
	RoomGraph* room = info->GetRoomGraph(); 
	// Miramos si tiene cargados un blackboard y un RoomGraph
	if(info != nullptr && room != nullptr){
		// Compruebo si se encuentra en la misma habitacion que el sensor
		output = room->IntoSameRoom(sig->GetPosition());
		// En el caso de que no este en la misma habitacion probamos si esta en una habitacion contigua
		if(!output) output = room->IntoNextRoom(sig->GetPosition());
	// En el caso de no tener cargados ninguno de los dos ( LOBBY ), lo damos por valido el sonido
	}else{
		output = true;
	}


	return output;
}
