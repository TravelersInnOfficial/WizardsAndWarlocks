#ifndef GRAILPARTICLE_H
#define GRAILPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>

class GrailParticle: public ParticleManager{
public:	
	GrailParticle();
	~GrailParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif