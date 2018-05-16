#include "./FountainParticle.h"

FountainParticle::FountainParticle(){
}

FountainParticle::~FountainParticle(){
}

void FountainParticle::InitParticle(Particle& p){
	int maxPosOffset = 1;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos = TOEvector3df(X, Y, Z);

	velocity = 2;
	int maxRandomVel = 15;
	X = direction.X * velocity + ((rand() % maxRandomVel)/10.0f - maxRandomVel/20.0f);
	Y = direction.Y * velocity + (rand() % maxRandomVel)/10.0f - maxRandomVel/20.0f;
	Z = direction.Z * velocity + ((rand() % maxRandomVel)/10.0f - maxRandomVel/20.0f);
	p.speed = TOEvector3df(X,Y,Z);

	int color = rand() % 2;
	p.r = 70; p.g = 70; p.b = 70;
	if(color == 0) p.r = 255;
	else p.b = 255;

	p.translation = TOEvector3df(0,0,0);

	// SIZE
	int maxSize = 5;
	p.size = (rand() % maxSize)/10.0f;

	p.rotation = (rand() % 360);
	p.life = 1.0f;
}

void FountainParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.X += p.speed.X * deltaTime * 1.5;
	p.speed.Z += p.speed.Z * deltaTime * 1.5;

	p.pos.X += p.speed.X * deltaTime;
	p.pos.Y += p.speed.Y * deltaTime;
	p.pos.Z += p.speed.Z * deltaTime;

	p.size += p.size * deltaTime;	
}

void FountainParticle::SetDirection(vector3df newDirection){
	direction = newDirection;
}