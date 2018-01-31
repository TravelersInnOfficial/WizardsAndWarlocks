#include "ObjectManager.h"
#include "./../Objects/AllPotions.h"
#include "./../NavMeshLoader.h"

ObjectManager* ObjectManager::instance = 0;

ObjectManager::ObjectManager(){
	navmesh = NULL;
	grail = NULL;
	readyZone = vector4df(-9999,-9999,-9999,-9999);
}

ObjectManager::~ObjectManager(){
	ClearMap();
	instance = 0;
}

ObjectManager* ObjectManager::GetInstance(){
	if(instance==0) instance = new ObjectManager();
	return instance;
}

// ===================================================================================================== //
//
// ADDERS
//
// ===================================================================================================== //

Block* ObjectManager::AddBlock(vector3df pos, vector3df size, vector3df rot, std::string texture){
	Block* b = new Block(pos, rot, size, texture);
	blocks.push_back(b);
	return b;
}


Prop* ObjectManager::AddProp(vector3df pos, vector3df size, vector3df rot, std::string model, std::string texture){
	Prop* p = new Prop(pos, rot, size, model, texture);
	props.push_back(p);
	return p;
}

void ObjectManager::AddSpawner(Alliance playerAlliance, vector3df TPosition){
	if(playerAlliance == ALLIANCE_WIZARD) wizardSpawn.push_back(TPosition);
	else warlockSpawn.push_back(TPosition);
}

void ObjectManager::AddReadyPoint(vector3df TPosition){
	// Guardamos el primer punto
	if(readyZone.X == -9999){
		readyZone.X = TPosition.X;
		readyZone.Y = TPosition.Z;
	}

	// Guardamos el segundo punto
	else if (readyZone.X2 == -9999){
		readyZone.X2 = TPosition.X;
		readyZone.Y2 = TPosition.Z;
		
		// Si el segundo punto es menor que el primero
		// Le damos la vuelta en X
		if(readyZone.X2 < readyZone.X){
			float aux = readyZone.X2;
			readyZone.X2 = readyZone.X;
			readyZone.X = aux;
		}

		// Si el segundo punto es menor que el primero
		// Y le damos la vuelta en Y
		if(readyZone.Y2 < readyZone.Y){
			float aux = readyZone.Y2;
			readyZone.Y2 = readyZone.Y;
			readyZone.Y = aux;
		}
	}
}

Door* ObjectManager::AddDoor(vector3df TPosition, vector3df TScale, vector3df TRotation, vector3df TCenter){
	Door* d = new Door(TPosition, TScale, TRotation, TCenter);
	doors.push_back(d);
	return d;
}

Grail* ObjectManager::AddGrail(vector3df TPosition, vector3df TScale, vector3df TRotation){
	if(grail==NULL) grail = new Grail(TPosition, TScale, TRotation);
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

Potion* ObjectManager::AddPotion(vector3df TPosition, vector3df TScale, vector3df TRotation, POTIONTYPE type){
	Potion* p = NULL;
	switch(type){
		case POTION_LIFE:
			p = (Potion*) new LifePotion(TPosition, TScale, TRotation);
			break;
		case POTION_MANA:
			p = (Potion*) new ManaPotion(TPosition, TScale, TRotation);
			break;
		case POTION_ICE:
			p = (Potion*) new IcePotion(TPosition, TScale, TRotation);
			break;
		case POTION_ELECTRIC:
			p = (Potion*) new ElectricPotion(TPosition, TScale, TRotation);
			break;
		case POTION_FIRE:
			p = (Potion*) new FirePotion(TPosition, TScale, TRotation);
			break;
		case POTION_POISON:
			p = (Potion*) new PoisonPotion(TPosition, TScale, TRotation);
			break;
		case POTION_DEFENSE:
			p = (Potion*) new DefensePotion(TPosition, TScale, TRotation);
			break;
		default:
			std::cout<<"POCION NO CONTROLADA"<<std::endl;
		break;
	}
	potions.push_back(p);
	return p;
}

Fountain* ObjectManager::AddFountain(vector3df TPosition, vector3df TScale, vector3df TRotation){
	Fountain* f = new Fountain(TPosition, TScale, TRotation);
	fountains.push_back(f);
	return f;
}

Npc* ObjectManager::AddNpc(vector3df TPosition, vector3df TScale, vector3df TRotation, NPCType type){
	Npc* n = NULL;
	switch(type){
		case(NPC_SELLER):{
			n = new NpcSeller(TPosition, TScale, TRotation);
			break;
		}
		case(NPC_SELECTOR):{
			n = new NpcSelector(TPosition, TScale, TRotation);
			break;
		}
		case(NPC_POWERUP):{
			//n = new NpcPowerUp(TPosition, TScale, TRotation);
			break;
		}
		default:{ break; }
	}
	if(n != NULL) npcs.push_back(n);
	return n;
}

Invocation* ObjectManager::AddInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation, InvoEnum type){
	Invocation* in = NULL;
	switch(type){
		case INVO_WALL:
			in = new InvocationWall(100, 10, TPosition, TScale, TRotation);
		break;

		case INVO_WIZARD:
			in = new Dummy(100, 10, true, TPosition, TScale, TRotation);
		break;

		case INVO_WARLOCK:
			in = new Dummy(100, 10, false, TPosition, TScale, TRotation);
		break;

		case INVO_TELEPORT:
			in = new BaseT(TPosition,  TScale, TRotation);
		break;
	}
	if(in!=NULL)invocations.push_back(in);
	return in;
}

DamageArea* ObjectManager::AddDamageArea(vector3df TPosition, vector3df TScale, vector3df TRotation, AreaEnum type){
	DamageArea* ar = NULL;

	switch(type){
		case AREA_ICE:
			ar = new IceArea(3, TPosition, TScale, TRotation);
		break;

		case AREA_POISON:
			ar = new PoisonArea(0, TPosition, TScale, TRotation);
		break;
	}
	if(ar!=NULL) damageAreas.push_back(ar);
	return ar;
}

void ObjectManager::AddNavmesh(std::string path){
	if(navmesh!=NULL){
		delete navmesh;
	}
	navmesh = new NavMesh();
	NavMeshLoader::LoadNavMeshGraph(navmesh, path);
}

// ===================================================================================================== //
//
// SENSE FUNCTIONS
//
// ===================================================================================================== //

void ObjectManager::SendAllSignal(){
	SendPotionSignal();
	SendFountainSignal();
	SendGrailSignal();
	SendInvocationSignal();
	SendSwitchSignal();
}

void ObjectManager::SendSwitchSignal(){
	int size = switchs.size();
	for(int i=0; i<size; i++){
		Switch* sw = switchs[i];
		sw->SendSignal();
	}
}

void ObjectManager::SendInvocationSignal(){
	int size = invocations.size();
	for(int i=0; i<size; i++){
		Invocation* in = invocations[i];
		in->SendSignal();
	}
}

void ObjectManager::SendGrailSignal(){
	if(grail!=NULL){
		grail->SendSignal();
	}
}

void ObjectManager::SendPotionSignal(){
	int size = potions.size();
	for(int i=0; i<size; i++){
		Potion* po = potions[i];
		po->SendSignal();
	}
}

void ObjectManager::SendFountainSignal(){
	int size = fountains.size();
	for(int i=0; i<size; i++){
		Fountain* fo = fountains[i];
		fo->SendSignal();
	}
}

// ===================================================================================================== //
//
// GETTERS
//
// ===================================================================================================== //

vector3df ObjectManager::GetRandomSpawnPoint(Alliance playerAlliance){
	vector3df toRet = vector3df(0,1,0);
	
	if(playerAlliance == ALLIANCE_WIZARD && wizardSpawn.size() > 0){
		int randIndex = rand() % wizardSpawn.size();
		toRet = wizardSpawn.at(randIndex);
	}

	else if(playerAlliance == ALLIANCE_WARLOCK && warlockSpawn.size() > 0){
		int randIndex = rand() % warlockSpawn.size();
		toRet = warlockSpawn.at(randIndex);
	}

	return(toRet);
}

vector4df ObjectManager::GetReadyZone(){ return readyZone; }

int ObjectManager::GetDoorVecPos(Door* door){
	int toRet = -1;
	for(int i = 0; i < doors.size() && toRet == -1; i++){
		Door* auxDoor = doors.at(i);
		if(auxDoor != NULL && door != NULL && auxDoor == door) toRet = i;
	}
	return toRet;
}

int ObjectManager::GetPotionVecPos(Potion* potion){
	int toRet = -1;
	for(int i = 0; i < potions.size() && toRet == -1; i++){
		Potion* auxPotion = potions.at(i);
		if(auxPotion != NULL && potion != NULL && auxPotion == potion) toRet = i;
	}
	return toRet;
}

NavMesh* ObjectManager::GetNavMesh(){return navmesh;}

// ===================================================================================================== //
//
// DRAWERS
//
// ===================================================================================================== //


void ObjectManager::DrawNpcMenu(){
	int size = npcs.size();
	for(int i=0; i<size; i++){
		Npc* n = npcs[i];
		n->Draw();
	}
}

void ObjectManager::DrawGrailGUI(){
	if(grail != NULL) grail->drawGUI();
}

// ===================================================================================================== //
//
// DELETERS
//
// ===================================================================================================== //


void ObjectManager::DeletePotion(Potion* potion){
	int size = potions.size();
	for(int i=size-1; i>=0; i--){
		Potion* p = potions[i];
		if(p == potion){
			potions.erase(potions.begin() + i);
			delete p;
		}
	}
}

void ObjectManager::DeleteBlock(Block* block){
	int size = blocks.size();
	for(int i=size-1; i>=0; i--){
		Block* b = blocks[i];
		if(b == block){
			blocks.erase(blocks.begin() + i);
			delete b;
		}
	}
}

// ===================================================================================================== //
//
// ACTIONS
//
// ===================================================================================================== //

void ObjectManager::ClearMap(){
	int size = blocks.size();
	for(int i=0; i<size; i++){
		Block* b = blocks[i];
		delete b;
	}
	blocks.clear();

	size = props.size();
	for(int i=0; i<size; i++){
		Prop* p = props[i];
		delete p;
	}
	props.clear();
	
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

	size = npcs.size();
	for(int i=0; i<size; i++){
		Npc* n = npcs[i];
		n->StopInteraction();
		delete n;
	}
	npcs.clear();

	size = invocations.size();
	for(int i=0; i<size; i++){
		Invocation* in = invocations[i];
		delete in;
	}
	invocations.clear();

	size = damageAreas.size();
	for(int i=0; i<size; i++){
		DamageArea* ar = damageAreas[i];
		delete ar;
	}
	damageAreas.clear();

	if(grail != NULL){
		delete grail;
		grail = NULL;
	}

	if(navmesh != NULL){
		delete navmesh;
		navmesh = NULL;
	}

	wizardSpawn.clear();
	warlockSpawn.clear();
	readyZone = vector4df(-9999,-9999,-9999,-9999);
}

bool ObjectManager::CheckIfWon(){
	bool toRet = false;
	if(grail != NULL) toRet = grail->CheckIfWon();
	return toRet;
}

// ===================================================================================================== //
//
// UPDATES
//
// ===================================================================================================== //

void ObjectManager::Update(float deltaTime){
	UpdateGrail(deltaTime);
	UpdateDoors();
	UpdateSwitchs();
	UpdatePotions();
	UpdateFountains(deltaTime);
	UpdateBlocks();
	UpdateNpcs();
	UpdateInvocations(deltaTime);
	UpdateDamageAreas(deltaTime);
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

void ObjectManager::UpdateNpcs(){
	int size = npcs.size();
	for(int i=0; i<size; i++){
		Npc* n = npcs[i];
		n->Update();
	}
}

void ObjectManager::StopInteractionsNPC(){
	int size = npcs.size();
	for(int i=0; i<size; i++){
		Npc* n = npcs[i];
		n->StopInteraction();
	}
}

void ObjectManager::UpdateInvocations(float deltaTime){
	int size = invocations.size();
	for(int i=size-1; i>=0; i--){
		Invocation* in = invocations[i];
		
		// En el caso de que el update sea False es que hay que eliminarlo
		if(!(in->Update(deltaTime))){
			invocations.erase(invocations.begin() + i);
			delete in;
		}
	}
}

void ObjectManager::UpdateDamageAreas(float deltaTime){
	int size = damageAreas.size();
	for(int i=size-1; i>=0; i--){
		DamageArea* ar = damageAreas[i];
		if(!(ar->Update(deltaTime))){
			damageAreas.erase(damageAreas.begin() + i);
			delete ar;
		}
	}
}

void ObjectManager::UseNetworkDoor(int doorVecPos){
	if(doorVecPos < doors.size()){
		Door* doorToInteract = doors.at(doorVecPos);
		if(doorToInteract != NULL) doorToInteract->NetInteract();
	}
}

void ObjectManager::UseNetworkPotion(int potionVecPos, Player* p){
	if(potionVecPos < potions.size()){
		Potion* potionToInteract = potions.at(potionVecPos);
		if(potionToInteract != NULL) potionToInteract->NetInteract(p);
	}
}

std::vector<Door*> ObjectManager::GetAllDoors(){
	return(doors);
}

std::vector<Potion*> ObjectManager::GetAllPotions(){
	return(potions);
}