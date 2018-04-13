#ifndef INVISIBLEPARTICLE_H
#define INVISIBLEPARTICLE_H
#include <TravelersOcularEngine/src/TOcularEngine/Elements/Particles/ParticleManager.h>

class InvisibleParticle: public ParticleManager{
public:	
	InvisibleParticle();
	~InvisibleParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif