/*#include "./ColoredParticle.h"

ColoredParticle::ColoredParticle(bool r, bool g, bool b){
	this->r = r;
	this->g = g;
	this->b = b;
}

ColoredParticle::~ColoredParticle(){
}

void ColoredParticle::InitParticle(Particle& p){
	p.translation = glm::vec3(0,0,0);

	float X = (rand() % 10)/10.0f - 0.5f;
	float Y = (rand() % 10)/10.0f - 0.5f;
	float Z = (rand() % 10)/10.0f - 0.5f;

	p.pos 	= glm::vec3(X, Y, Z);

	Y = (rand() % 4)/10.0f - 0.2f;
	X = (rand() % 2)/10.0f - 0.1f;
	Z = (rand() % 2)/10.0f - 0.1f;

	p.speed = glm::vec3(X,Y,Z);

	if(r) p.r = 255;
	else p.r = (unsigned char)(rand() % 255);
	
	if(g) p.g = 255;
	else p.g = (unsigned char)(rand() % 255);
	
	if(b) p.b = 255;
	else p.b = (unsigned char)(rand() % 255);
	
	p.size = (rand() % 50)/10.0f;
	p.rotation = (rand() % 360);
	p.life = 20.0f;
}

void ColoredParticle::UpdateParticle(Particle& p, float deltaTime){
	p.speed += glm::vec3(0.0f,0.25f, 0.0f) * deltaTime * 0.5f;
	p.pos += p.speed * deltaTime;
}
*/