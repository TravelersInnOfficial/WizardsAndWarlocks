#include "DamageArea.h"
#include "./../Players/Player.h"

DamageArea::DamageArea(int dam, vector3df TPosition, vector3df TScale, vector3df TRotation){
	damage = dam;
	casting = 0.2f;
	currentCast = 0.0f;
	activated = true;
	CreateDamageArea(TPosition, TScale, TRotation);
}
	
DamageArea::~DamageArea(){
	bt_body->Erase();

	delete bt_body;
}

void DamageArea::CreateDamageArea(vector3df TPosition, vector3df TScale, vector3df TRotation){
	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_GhostObject();
	bt_body->CreateGhostBox(TPosition, TRotation, HalfExtents);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

bool DamageArea::Update(float deltaTime){
	if(!activated){
		return false;
	}
	if(currentCast<=0){
		currentCast = casting;
	}
	currentCast -= deltaTime;
	return true;
}

	
void DamageArea::Update(){
}
	
void DamageArea::Contact(void* punt, EntityEnum tipo){
	if(currentCast<=0){
		if(tipo==EENUM_PLAYER){
	        Player* p = (Player*)punt;
	        p->ChangeHP(-damage);
	    }
	}
}

void DamageArea::SetPosition(vector3df pos){
	bt_body->SetPosition(pos);
}

void DamageArea::SetRotation(vector3df rot){
	bt_body->Rotate(rot);
}

void DamageArea::Deactivate(){
	activated = false;
}
