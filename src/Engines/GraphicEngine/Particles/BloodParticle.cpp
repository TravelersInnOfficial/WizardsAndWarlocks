#include "./BloodParticle.h"

BloodParticle::BloodParticle(){
}

BloodParticle::~BloodParticle(){
}

void BloodParticle::InitParticle(Particle& p){

	int maxPosOffset = 3;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos = toe::core::TOEvector3df(X, Y, Z);

	int maxVel = 5;
	X = (rand() % maxVel)/10.0f - maxVel/20.0f;
	Y = maxVel;
	Z = (rand() % maxVel)/10.0f - maxVel/20.0f;
	p.speed = toe::core::TOEvector3df(X*2,Y,Z*2);

	int color = 240; p.r = color; p.g = color; p.b = color;

	p.translation = toe::core::TOEvector3df(0,0,0);

	// SIZE
	int maxSize = 3;
	p.size = (rand() % maxSize)/10.0f;

	p.rotation = (rand() % 360);
	p.life = 5.0f;
}

void BloodParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.Y -= deltaTime*10;
	p.pos.X += p.speed.X * deltaTime;
	p.pos.Y += p.speed.Y * deltaTime;
	p.pos.Z += p.speed.Z * deltaTime;
}