#include "IceArea.h"

#include <PhysicsEngine/BT_GhostObject.h>
#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GParticle.h>
#include <GraphicEngine/GBody.h>
#include "./../Players/Player.h"
#include <ColliderMasks.h>
#include <Constants.h>

IceArea::IceArea(float dam, vector3df TPosition, vector3df TScale, vector3df TRotation)
:DamageArea(dam, 0.2f, TPosition, TScale, TRotation){
	activated = true;
	particle = nullptr;
	CreateIceArea(TPosition, TScale, TRotation);
}
	
IceArea::~IceArea(){
	if(particle != nullptr) delete particle;
}

void IceArea::SetPosition(vector3df pos){
	bt_body->SetPosition(pos);
	m_areaNode->setPosition(pos);
	if(particle != nullptr){
		WindParticle* pm = (WindParticle*)particle->GetPM();

		vector3df rotation = m_areaNode->getRotation();

		float rotX = -rotation.X * M_PI / 180.0f;
		float rotY = rotation.Y * M_PI / 180.0f;
		vector3df newDir = vector3df(sin(rotY)*cos(rotX), sin(rotX), cos(rotY)*cos(rotX));
		pm->SetDirection(newDir);

		float dist = -1.0f;
		vector3df TPosition = pos;
		TPosition.X = TPosition.X + sin(rotY)*cos(rotX)*dist;
		TPosition.Y = TPosition.Y + sin(rotX)*dist;
		TPosition.Z = TPosition.Z + cos(rotY)*cos(rotX)*dist;
		particle->SetPosSpecial(TPosition);
	}
}

bool IceArea::Update(float deltaTime){
	if(particle != nullptr) particle->Update();

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
	GraphicEngine* g_engine = GraphicEngine::getInstance();

	// Cargamos el cubo
    m_areaNode = g_engine->addCube2Scene(TPosition, TRotation, TScale);

    // Aplicamos Material unlit y Textura
    if (m_areaNode) {
        if(!GraphicEngine::getInstance()->GetParticleActive()) m_areaNode->setMaterialTexture(0, "./../assets/textures/marbre5.jpg");
		else m_areaNode->setMaterialTexture(0, "./../assets/textures/none.png");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X*0.5, TScale.Y*0.5, TScale.Z*0.5);
	bt_body = new BT_GhostObject();
	bt_body->CreateGhostBox(TPosition, TRotation, HalfExtents, vector3df(0,0,0), C_DAMAGEAREA, damageareaCW);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);

	if(GraphicEngine::getInstance()->GetParticleActive()){
		float dist = -0.75f;
		TPosition.X = TPosition.X + sin(TRotation.Y)*dist;
		TPosition.Z = TPosition.Z + cos(TRotation.Y)*dist;

		particle = new GParticle(TPosition);
		particle->SetRot(TRotation);
		particle->SetTexture("./../assets/textures/particles/FreezeParticle.png");
		particle->SetType(WIND_PARTICLE);
		particle->SetQuantityPerSecond(200);
	}
}
