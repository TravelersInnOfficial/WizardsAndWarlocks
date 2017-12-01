#include "ManagerObject.h"

ManagerObject* ManagerObject::instance = 0;

ManagerObject::ManagerObject(){
	grail = NULL;
}

ManagerObject::~ManagerObject(){
	int size = blocks.size();
	for(int i=0; i<size; i++){
		Block* b = blocks[i];
		delete b;
	}
	doors.clear();
	
	size = doors.size();
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

	delete grail;
}

ManagerObject* ManagerObject::GetInstance(){
	if(instance==0){
		instance = new ManagerObject();
	}
	return instance;
}

Block* ManagerObject::AddBlock(vector3df pos, vector3df size, vector3df rot, int texture){
	Block* b = new Block(pos, rot, size, texture);
	blocks.push_back(b);
	return b;
}

Door* ManagerObject::AddDoor(){
	Door* d = new Door();
	doors.push_back(d);
	return d;
}

Grail* ManagerObject::AddGrail(){
	if(grail==NULL){
		grail = new Grail();
	}
	return grail;
}

Switch* ManagerObject::AddSwitch(Door* d){
	Switch* s = new Switch();
	s->SetDoor(d);
	switchs.push_back(s);
	return s;
}

Potion* ManagerObject::AddPotion(vector3df pos){
	Potion* p = new Potion(pos);
	potions.push_back(p);
	return p;
}

Fountain* ManagerObject::AddFountain(){
	Fountain* f = new Fountain();
	fountains.push_back(f);
	return f;
}

void ManagerObject::DeletePotion(Potion* potion){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		if(p == potion){
			potions.erase(potions.begin() + i);
			delete p;
		}
	}
}

void ManagerObject::Update(){
	UpdateGrail();
	UpdateDoors();
	UpdateSwitchs();
	UpdatePotions();
	UpdateFountains();
	UpdateBlocks();
}

void ManagerObject::UpdateGrail(){
	if(grail!=NULL){
		grail->Update();
	}
}

void ManagerObject::UpdateDoors(){
	int size = doors.size();
	for(int i=0; i<size; i++){
		Door* d = doors[i];
		d->Update();
	}
}

void ManagerObject::UpdateSwitchs(){
	int size = switchs.size();
	for(int i=0; i<size; i++){
		Switch* s = switchs[i];
		s->Update();
	}
}

void ManagerObject::UpdatePotions(){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		p->Update();
	}
}

void ManagerObject::UpdateFountains(){
	int size = fountains.size();
	for(int i=0; i<size; i++){
		Fountain* f = fountains[i];
		f->Update();
	}
}

void ManagerObject::UpdateBlocks(){
	int size = blocks.size();
	for(int i=0; i<size; i++){
		Block* b = blocks[i];
		b->Update();
	}
}