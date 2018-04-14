#ifndef POISONPARTICLE_H
#define POISONPARTICLE_H
#include <TravelersOcularEngine/src/TOcularEngine/Elements/Particles/ParticleManager.h>

class PoisonParticle: public ParticleManager{
public:	
	PoisonParticle();
	~PoisonParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif