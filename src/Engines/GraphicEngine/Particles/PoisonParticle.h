#ifndef POISONPARTICLE_H
#define POISONPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>

class PoisonParticle: public ParticleManager{
public:	
	PoisonParticle();
	~PoisonParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif