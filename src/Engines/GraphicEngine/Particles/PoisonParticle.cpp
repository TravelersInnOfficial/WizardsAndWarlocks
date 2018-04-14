#include "./PoisonParticle.h"

PoisonParticle::PoisonParticle(){
}

PoisonParticle::~PoisonParticle(){
}

void PoisonParticle::InitParticle(Particle& p){

	/*float X = (rand() % 10)/10.0f - 0.5f;
	float Y = (rand() % 10)/10.0f - 0.5f;
	float Z = (rand() % 10)/10.0f - 0.5f;
	p.pos 	= toe::core::TOEvector3df(X, Y, Z);

	Y = (rand() % 5)/10.0f;
	X = (rand() % 5)/10.0f;
	Z = (rand() % 5)/10.0f;
	p.speed = toe::core::TOEvector3df(X,Y,Z);

	//int color = (unsigned char)(rand() % 255 + 240);
	int color = 255;
	p.r = color;
	p.g = color;
	p.b = color;

	p.translation = toe::core::TOEvector3df(0,0,0);
	p.size = (rand() % 3)/6.0f;
	p.rotation = (rand() % 360);
	p.life = 10.0f;*/

	p.translation = toe::core::TOEvector3df(0,0,0);

	// INITIAL POSITION
	int maxPosOffset = 10;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos = toe::core::TOEvector3df(X, Y, Z);

	// VELOCITY
	int maxVel = 20;
	X = (rand() % maxVel)/10.0f - maxVel/20.0f;
	Y = (rand() % maxVel)/10.0f - maxVel/20.0f;
	Z = (rand() % maxVel)/10.0f - maxVel/20.0f;
	p.speed = toe::core::TOEvector3df(X,Y,Z);

	// COLOR
	p.r = (unsigned char)(rand() % 255);
	p.g = (unsigned char)(rand() % 255);
	p.b = (unsigned char)(rand() % 255);

	// SIZE
	int maxSize = 10;
	p.size = (rand() % maxSize)/10.0f;

	// ROTATION & LIFE
	p.rotation = (rand() % 360);
	p.life = 20.0f;
}

void PoisonParticle::UpdateParticle(Particle& p, float deltaTime){
	//p.speed.X += deltaTime * 0.5f;
	//p.speed.Y += deltaTime * 0.5f;
	//p.speed.Z += deltaTime * 0.5f;
	p.pos.X += p.speed.X * deltaTime;
	p.pos.Y += p.speed.Y * deltaTime;
	p.pos.Z += p.speed.Z * deltaTime;
}