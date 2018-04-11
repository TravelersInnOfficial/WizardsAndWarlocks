#include "./TrapParticle.h"

TrapParticle::TrapParticle(){
}

TrapParticle::~TrapParticle(){
}

void TrapParticle::InitParticle(Particle& p){

	float X = (rand() % 10)/10.0f - 0.5f;
	float Y = 0.15f;
	float Z = (rand() % 10)/10.0f - 0.5f;
	p.pos 	= toe::core::TOEvector3df(X, Y, Z);

	Y = (rand() % 2)/10.0f;
	X = (rand() % 2)/10.0f;
	Z = (rand() % 2)/10.0f;
	p.speed = toe::core::TOEvector3df(X,Y,Z);

	int color = (unsigned char)(rand() % 255 + 240);
	p.r = color;
	p.g = color;
	p.b = color;

	p.translation = toe::core::TOEvector3df(0,0,0);
	p.size = (rand() % 3)/20.0f;
	p.rotation = (rand() % 360);
	p.life = 2.5f;
}

void TrapParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.Y += deltaTime * 0.5f * 0.25f;
	p.pos.Y += p.speed.Y * deltaTime;
}