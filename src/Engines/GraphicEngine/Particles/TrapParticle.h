/*#ifndef COLOREDPARTICLE_H
#define COLOREDPARTICLE_H

#include <glm/vec3.hpp>
#include "./TOcularEngine/Elements/Particles/ParticleManager.h"

class ColoredParticle: public ParticleManager{
public:	
	ColoredParticle(bool r, bool g, bool b);
	~ColoredParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:
	bool r;
	bool g;
	bool b;

};

#endif*/