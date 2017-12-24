#include "Signal.h"
#include "Modality.h"

Signal::Signal(Signal* sig){
	id = sig->GetId();
	code = sig->GetCode();
	emisor = sig->GetEmisor();
	kinematic = sig->GetKinematic();
	strength = sig->GetStrength();
	modality = new SightModality(sig->GetModality());
}

Signal::Signal(int i, AI_code name, void* emi, float str, Kinematic kin){
	id = i;
	code = name;
	emisor = emi;
	strength = str;
	kinematic = kin;
	modality = new SightModality(10.0f, 1.0f, 0.1f);
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

void* Signal::GetEmisor(){
	return emisor;
}

vector3df Signal::GetPosition(){
	return kinematic.position;
}