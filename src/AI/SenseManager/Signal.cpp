#include "Signal.h"
#include "Modality.h"

Signal::Signal(Signal* sig){
	id = sig->GetId();
	code = sig->GetCode();
	kinematic = sig->GetKinematic();
	strength = sig->GetStrength();

	AI_modalities type = sig->GetModality()->GetType();
	switch(type){
		case AI_SIGHT:
			modality = new SightModality(sig->GetModality());
			break;
		case AI_HEARING:
			modality = new HearingModality(sig->GetModality());
			break;
	}
}

Signal::Signal(int i, AI_code name, float str, Kinematic kin, AI_modalities mod){
	id = i;
	code = name;
	strength = str;
	kinematic = kin;
	switch(mod){
		case AI_SIGHT:
			modality = new SightModality(10.0f, 1.0f, 0.1f);
			break;
		case AI_HEARING:
			modality = new HearingModality(10.0f, 1.0f, 0.1f);
			break;
		default:
			modality = new HearingModality(10.0f, 1.0f, 0.1f);
			break;
	}
}

Signal::~Signal(){
	delete modality;
}

Kinematic Signal::GetKinematic(){
	return kinematic;
}

Modality* Signal::GetModality(){
	return modality;
}

float Signal::GetStrength(){
	return strength;
}

AI_code Signal::GetCode(){
	return code;
}

int Signal::GetId(){
	return id;
}

vector3df Signal::GetPosition(){
	return kinematic.position;
}