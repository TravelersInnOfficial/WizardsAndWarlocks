#include "./EffectParticle.h"

EffectParticle::EffectParticle(){
}

EffectParticle::~EffectParticle(){
}

void EffectParticle::InitParticle(Particle& p){

	float X = (rand() % 10)/10.0f - 0.5f;
	float Y = (rand() % 20)/10.0f - 0.5f;
	float Z = (rand() % 10)/10.0f - 0.5f;
	p.pos 	= toe::core::TOEvector3df(X, Y, Z);

	Y = (rand() % 2)/10.0f;
	X = (rand() % 2)/10.0f;
	Z = (rand() % 2)/10.0f;
	p.speed = toe::core::TOEvector3df(X,Y,Z);

	//int color = (unsigned char)(rand() % 255 + 240);
	int color = 255;
	p.r = color;
	p.g = color;
	p.b = color;

	p.translation = toe::core::TOEvector3df(0,0,0);
	p.size = (rand() % 3)/10.0f;
	p.rotation = (rand() % 360);
	p.life = 0.25f;
}

void EffectParticle::UpdateParticle(Particle& p, float deltaTime){
	//p.speed.Y += deltaTime * 0.5f * 0.25f;
	p.pos.Y += p.speed.Y * deltaTime;
}