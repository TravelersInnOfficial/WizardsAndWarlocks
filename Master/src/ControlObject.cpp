#include "ControlObject.h"

ControlObject* ControlObject::instance = 0;

ControlObject::ControlObject(){
	grial = NULL;
}

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

	size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		delete p;
	}
	potions.clear();

	size = fountains.size();
	for(int i=0; i<size; i++){
		Fountain* f = fountains[i];
		delete f;
	}
	fountains.clear();

	delete grial;
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

Grial* ControlObject::AddGrial(){
	if(grial==NULL){
		grial = new Grial();
	}
	return grial;
}

Switch* ControlObject::AddSwitch(Door* d){
	Switch* s = new Switch();
	s->SetDoor(d);
	switchs.push_back(s);
	return s;
}

Potion* ControlObject::AddPotion(vector3df pos){
	Potion* p = new Potion(pos);
	potions.push_back(p);
	return p;
}

Fountain* ControlObject::AddFountain(){
	Fountain* f = new Fountain();
	fountains.push_back(f);
	return f;
}

void ControlObject::DeletePotion(Potion* potion){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		if(p == potion){
			potions.erase(potions.begin() + i);
			delete p;
		}
	}
}

void ControlObject::Update(){
	UpdateGrial();
	UpdateDoors();
	UpdateSwitchs();
	UpdatePotions();
}

void ControlObject::UpdateGrial(){
	if(grial!=NULL){
		grial->Update();
	}
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

void ControlObject::UpdatePotions(){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		p->Update();
	}
}

void ControlObject::UpdateFountains(){
	int size = fountains.size();
	for(int i=0; i<size; i++){
		Fountain* f = fountains[i];
		f->Update();
	}
}
