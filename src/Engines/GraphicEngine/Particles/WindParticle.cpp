#include "./WindParticle.h"

WindParticle::WindParticle(){
}

WindParticle::~WindParticle(){
}

void WindParticle::InitParticle(Particle& p){

	int maxPosOffset = 1;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos =  TOEvector3df(X, Y, Z);

	velocity = 2;
	int maxRandomVel = 15;
	X = direction.X * velocity + ((rand() % maxRandomVel)/10.0f - maxRandomVel/20.0f);
	Y = (rand() % maxRandomVel)/10.0f - maxRandomVel/20.0f;
	Z = direction.Z * velocity + ((rand() % maxRandomVel)/10.0f - maxRandomVel/20.0f);
	p.speed =  TOEvector3df(X,Y,Z);

	int color = 255; p.r = color; p.g = color; p.b = color;
	p.translation =  TOEvector3df(0,0,0);

	// SIZE
	int maxSize = 3;
	p.size = (rand() % maxSize)/10.0f;

	p.rotation = (rand() % 360);
	p.life = 1.0f;
}

void WindParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.X += p.speed.X * deltaTime * 1.5;
	p.speed.Z += p.speed.Z * deltaTime * 1.5;

	p.pos.X += p.speed.X * deltaTime;
	p.pos.Y += p.speed.Y * deltaTime;
	p.pos.Z += p.speed.Z * deltaTime;

	p.size += p.size * deltaTime;	
}

void WindParticle::SetDirection(vector3df newDirection){
	direction = newDirection;
}