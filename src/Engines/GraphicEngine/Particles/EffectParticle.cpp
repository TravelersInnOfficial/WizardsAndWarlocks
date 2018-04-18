#include "./EffectParticle.h"

EffectParticle::EffectParticle(){
}

EffectParticle::~EffectParticle(){
}

void EffectParticle::InitParticle(Particle& p){

	int maxPosOffset = 15;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = 0.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos = toe::core::TOEvector3df(X, Y, Z);


	int maxVel = 2;
	X = (rand() % maxVel)/10.0f - maxVel/20.0f;
	Y = maxVel;
	Z = (rand() % maxVel)/10.0f - maxVel/20.0f;
	p.speed = toe::core::TOEvector3df(X, Y, Z);

	int color = 255;
	p.r = color;
	p.g = color;
	p.b = color;

	p.translation = toe::core::TOEvector3df(0,-1.4,0);
	
	// SIZE
	int maxSize = 5;
	p.size = (rand() % maxSize)/10.0f;

	p.rotation = (rand() % 360);
	p.life = 1.8f;
}

void EffectParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.Y += deltaTime * 0.5f * 0.25f;
	p.pos.Y += p.speed.Y * deltaTime;
	p.size -= p.size * deltaTime * 0.5f;
	p.r -= p.r * deltaTime * 0.25f;
	p.g -= p.g * deltaTime * 0.25f;
	p.b -= p.b * deltaTime * 0.25f;
}