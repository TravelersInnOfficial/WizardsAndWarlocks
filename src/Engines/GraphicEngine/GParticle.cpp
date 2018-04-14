#include "GParticle.h"
#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>
#include "./../../Managers/StateManager.h"
#include <ParticleData.h>

GParticle::GParticle(vector3df position){
	SceneManager* sm = GraphicEngine::getInstance()->privateSManager;
	toe::core::TOEvector3df pos = toe::core::TOEvector3df(position.X, position.Y, position.Z);
	ps = sm->AddParticleSystem(pos, toe::core::TOEvector3df(0,0,0), toe::core::TOEvector3df(1,1,1));
}

GParticle::~GParticle(){
	SceneManager* sm = GraphicEngine::getInstance()->privateSManager;
	sm->DeleteMesh(ps);
}

void GParticle::SetQuantityPerSecond(int particles){
	ps->SetNewPerSecond(particles);
}

void GParticle::SetTexture(std::string path){
	ps->SetTexture(path);
}

void GParticle::SetType(PARTICLE_TYPE type){
	switch(type){
		case TRAP_PARTICLE:
			ps->SetManager(new TrapParticle());
			break;
		case INVISIBLE_PARTICLE:
			ps->SetManager(new InvisibleParticle()); 
			break;
		case PROJECTILE_PARTICLE:
			ps->SetManager(new ProjectileParticle()); 
			break;
		case EFFECT_PARTICLE:
			ps->SetManager(new EffectParticle()); 
			break;
		case POISON_PARTICLE:
			ps->SetManager(new PoisonParticle()); 
			break;
		case BLOOD_PARTICLE:
			ps->SetManager(new BloodParticle()); 
			break;
		case WIND_PARTICLE:
			//ps->SetManager(new WindParticle()); 
			break;
		default:;
	}
}

void GParticle::SetPos(vector3df position){
	toe::core::TOEvector3df pos = toe::core::TOEvector3df(position.X, position.Y, position.Z);
	ps->SetTranslate(pos);
}

void GParticle::Update(){
	float dt = StateManager::GetInstance()->GetDeltaTime();
	ps->Update(dt);
}