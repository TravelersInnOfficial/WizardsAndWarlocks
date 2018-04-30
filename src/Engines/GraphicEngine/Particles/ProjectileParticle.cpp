#include "./ProjectileParticle.h"

ProjectileParticle::ProjectileParticle(){
}

ProjectileParticle::~ProjectileParticle(){
}

void ProjectileParticle::InitParticle(Particle& p){

	int maxPosOffset = 7;
	float X = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Y = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	float Z = (rand() % maxPosOffset)/10.0f - maxPosOffset/20.0f;
	p.pos =  TOEvector3df(X, Y, Z);

	int maxVel = 5;
	X = (rand() % maxVel)/10.0f - maxVel/20.0f;
	Y = (rand() % maxVel)/10.0f - maxVel/20.0f;
	Z = (rand() % maxVel)/10.0f - maxVel/20.0f;
	p.speed =  TOEvector3df(X,Y,Z);

	//int color = (unsigned char)(rand() % 255 + 240);
	int color = 255;
	p.r = color;
	p.g = color;
	p.b = color;

	p.translation =  TOEvector3df(0,0,0);
	p.size = (rand() % 3)/6.0f;
	p.rotation = (rand() % 360);
	p.life = 0.5f;
}

void ProjectileParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed.X += deltaTime * 0.5f * 0.25f;
	p.speed.Y += deltaTime * 0.5f * 0.25f;
	p.speed.Z += deltaTime * 0.5f * 0.25f;
	p.pos.X += p.speed.X * deltaTime;
	p.pos.Y += p.speed.Y * deltaTime;
	p.pos.Z += p.speed.Z * deltaTime;
}