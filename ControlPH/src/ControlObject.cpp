#include "ControlObject.h"

ControlObject* ControlObject::instance = 0;

ControlObject::ControlObject(){}

ControlObject::~ControlObject(){
	int size = doors.size();
	for(int i=0; i<size; i++){
		Door* d = doors[i];
		delete d;
	}
	doors.clear();
	size = switchs.size();
	for(int i=0; i<size; i++){
		Switch* s = switchs[i];
		delete s;
	}
	switchs.clear();
}

ControlObject* ControlObject::GetInstance(){
	if(instance==0){
		instance = new ControlObject();
	}
	return instance;
}

Door* ControlObject::AddDoor(){
	Door* d = new Door();
	doors.push_back(d);
	return d;
}

Switch* ControlObject::AddSwitch(Door* d){
	Switch* s = new Switch();
	s->SetDoor(d);
	switchs.push_back(s);
	return s;
}

void ControlObject::Update(){
	UpdateDoors();
	UpdateSwitchs();
}

void ControlObject::UpdateDoors(){
	int size = doors.size();
	for(int i=0; i<size; i++){
		Door* d = doors[i];
		d->Update();
	}
}

void ControlObject::UpdateSwitchs(){
	int size = switchs.size();
	for(int i=0; i<size; i++){
		Switch* s = switchs[i];
		s->Update();
	}
}