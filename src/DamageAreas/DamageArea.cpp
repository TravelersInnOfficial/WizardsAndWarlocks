#include "DamageArea.h"
#include "./../Players/Player.h"

DamageArea::DamageArea(float dam, float cast, vector3df TPosition, vector3df TScale, vector3df TRotation){
	damage = dam;
	casting = cast;
	currentCast = 0.0f;
	clase = EENUM_DAMAGEAREA;
}
	
DamageArea::~DamageArea(){
	delete m_areaNode;
	delete bt_body;
}

bool DamageArea::Update(float deltaTime){
	std::cout<<"DAMAGE AREA BASICO"<<std::endl;
	return true;
}
	
void DamageArea::Update(){
}
	
void DamageArea::Contact(void* punt, EntityEnum tipo){
	std::cout<<"CONTACT DAMAGE AREA"<<std::endl;
}

void DamageArea::SetPosition(vector3df pos){
	bt_body->SetPosition(pos);
	m_areaNode->setPosition(pos);
}

void DamageArea::SetRotation(vector3df rot){
	bt_body->Rotate(rot);
	m_areaNode->setRotation(rot);
}

