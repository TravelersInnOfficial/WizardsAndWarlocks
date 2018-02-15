#include "IceArea.h"
#include "./../Players/Player.h"
#include <ColliderMasks.h>


IceArea::IceArea(float dam, vector3df TPosition, vector3df TScale, vector3df TRotation)
:DamageArea(dam, 0.2f, TPosition, TScale, TRotation){
	activated = true;
	CreateIceArea(TPosition, TScale, TRotation);
}
	
IceArea::~IceArea(){}

bool IceArea::Update(float deltaTime){
	if(!activated) return false;
	if(currentCast<=0) currentCast = casting;
	currentCast -= deltaTime;
	return true;
}
	
void IceArea::Contact(void* punt, EntityEnum tipo){
	if(currentCast<=0){
		if(tipo==EENUM_PLAYER){
	        Player* p = (Player*)punt;
	        if(p->GetId() == emisor) return;
	        p->ChangeHP(-damage);
	    }
	}
}

void IceArea::Deactivate(){
	activated = false;
}

void IceArea::SetEmisor(int emi){
	emisor = emi;
}

void IceArea::CreateIceArea(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* engine = GraphicEngine::getInstance();

	// Cargamos el cubo
    m_areaNode = engine->addCube2Scene(TPosition, TRotation, TScale);

    // Aplicamos Material unlit y Textura
    if (m_areaNode) {
        m_areaNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        m_areaNode->setMaterialTexture(0, "./../assets/textures/marbre5.jpg");
    }
	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_GhostObject();
	bt_body->CreateGhostBox(TPosition, TRotation, HalfExtents, vector3df(0,0,0), C_DAMAGEAREA, damageareaCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}
