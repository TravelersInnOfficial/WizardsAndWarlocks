#include "PoisonArea.h"
#include "./../Managers/EffectManager.h"
#include "./../Players/Player.h"

PoisonArea::PoisonArea(int dam, vector3df TPosition, vector3df TScale, vector3df TRotation)
:DamageArea(dam, 0.2f, TPosition, TScale, TRotation){
	ghostScale = 1;
	emisor = NO_ALLIANCE;
	CreatePoisonArea(TPosition, TScale, TRotation);
}
	
PoisonArea::~PoisonArea(){}

bool PoisonArea::Update(float deltaTime){
	// Gas bomb update
    ghostScale += 0.010;
    bt_body->SetScale(ghostScale*1.4);
    m_areaNode->setScale(vector3df(ghostScale));

    if(ghostScale > 6.0f){
       return false;
    }
	return true;
}
	
void PoisonArea::Contact(void* punt, EntityEnum tipo){
	if(currentCast<=0){
		if(tipo==EENUM_PLAYER){
	        Player* p = (Player*)punt;
	        if(p->GetAlliance() == emisor) return;
	        EffectManager::GetInstance()->AddEffect(p, WEAK_POISONED);
	    }
	}
}

void PoisonArea::SetAlliance(Alliance emi){
	emisor = emi;
}

void PoisonArea::CreatePoisonArea(vector3df TPosition, vector3df TScale, vector3df TRotation){    
    // Cargamos el modelo mierder
	m_areaNode = GraphicEngine::getInstance()->addObjMeshSceneNode("../assets/modelos/bomb.obj");
    m_areaNode->setPosition(TPosition);
	m_areaNode->setScale(TScale);

	// Aplicamos Material unlit y Textura
	if (m_areaNode) {
		m_areaNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
		m_areaNode->setMaterialTexture(0, "./../assets/textures/projectils/SPELL_POISON.png");
	}

   	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_GhostObject();
	bt_body->CreateGhostBox(TPosition, TRotation, HalfExtents);
    bt_body->AssignPointer(this);
}
