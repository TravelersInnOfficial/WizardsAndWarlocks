#include "./GrailParticle.h"

GrailParticle::GrailParticle(){
}

GrailParticle::~GrailParticle(){
}

void GrailParticle::InitParticle(Particle& p){

	int maxPosOffset = 50;
	int maxPosOffsetY = 2;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = (rand() % maxPosOffsetY)/10.0f - maxPosOffsetY/20.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos =  TOEvector3df(X, Y, Z);

	Y = (rand() % 2)/10.0f;
	X = (rand() % 2)/10.0f;
	Z = (rand() % 2)/10.0f;
	p.speed =  TOEvector3df(X,Y,Z);

	//int color = (unsigned char)(rand() % 255 + 240);
	p.r = 60;
	p.g = 200;
	p.b = 120;
	
	// SIZE
	int maxSize = 5;
	p.size = (rand() % maxSize)/10.0f;

	p.translation =  TOEvector3df(0,0,0);
	p.rotation = (rand() % 360);
	p.life = 9.5f;
}

void GrailParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.Y += deltaTime * 0.5f * 0.25f;
	p.pos.Y += p.speed.Y * deltaTime;
}