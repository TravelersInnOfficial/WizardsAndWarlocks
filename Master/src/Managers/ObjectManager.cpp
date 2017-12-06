#include "ObjectManager.h"

ObjectManager* ObjectManager::instance = 0;

ObjectManager::ObjectManager(){
	grail = NULL;
}

ObjectManager::~ObjectManager(){
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

ObjectManager* ObjectManager::GetInstance(){
	if(instance==0) instance = new ObjectManager();
	return instance;
}

Block* ObjectManager::AddBlock(vector3df pos, vector3df size, vector3df rot, std::string texture){
	Block* b = new Block(pos, rot, size, texture);
	blocks.push_back(b);
	return b;
}

Door* ObjectManager::AddDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter){
	Door* d = new Door(TPosition, TScale, TRotation, TCenter);
	doors.push_back(d);
	return d;
}

Grail* ObjectManager::AddGrail(){
	if(grail==NULL) grail = new Grail();
	return grail;
}

Switch* ObjectManager::AddSwitch(Door* d, vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter){
	Switch* s = new Switch(TPosition, TScale, TRotation, TCenter);
	s->SetDoor(d);
	switchs.push_back(s);
	return s;
}

Switch* ObjectManager::AddSwitch(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter){
	Switch* s = new Switch(TPosition, TScale, TRotation, TCenter);
	switchs.push_back(s);
	return s;
}

Potion* ObjectManager::AddPotion(vector3df TPosition, vector3df TScale, vector3df TRotation){
	Potion* p = new Potion(TPosition, TScale, TRotation);
	potions.push_back(p);
	return p;
}

Fountain* ObjectManager::AddFountain(){
	Fountain* f = new Fountain();
	fountains.push_back(f);
	return f;
}

void ObjectManager::DeletePotion(Potion* potion){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		if(p == potion){
			potions.erase(potions.begin() + i);
			delete p;
		}
	}
}

void ObjectManager::Update(float deltaTime){
	UpdateGrail(deltaTime);
	UpdateDoors();
	UpdateSwitchs();
	UpdatePotions();
	UpdateFountains(deltaTime);
	UpdateBlocks();
}

void ObjectManager::UpdateGrail(float deltaTime){
	if(grail!=NULL) grail->Update(deltaTime);
}

void ObjectManager::UpdateDoors(){
	int size = doors.size();
	for(int i=0; i<size; i++){
		Door* d = doors[i];
		d->Update();
	}
}

void ObjectManager::UpdateSwitchs(){
	int size = switchs.size();
	for(int i=0; i<size; i++){
		Switch* s = switchs[i];
		s->Update();
	}
}

void ObjectManager::UpdatePotions(){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* p = potions[i];
		p->Update();
	}
}

void ObjectManager::UpdateFountains(float deltaTime){
	int size = fountains.size();
	for(int i=0; i<size; i++){
		Fountain* f = fountains[i];
		f->Update(deltaTime);
	}
}

void ObjectManager::UpdateBlocks(){
	int size = blocks.size();
	for(int i=0; i<size; i++){
		Block* b = blocks[i];
		b->Update();
	}
}