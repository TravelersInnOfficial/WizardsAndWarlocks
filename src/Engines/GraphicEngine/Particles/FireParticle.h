#ifndef FIREPARTICLE_H
#define FIREPARTICLE_H
#include <TravelersOcularEngine/src/TOcularEngine/Elements/Particles/ParticleManager.h>

class FireParticle: public ParticleManager{
public:	
	FireParticle();
	~FireParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif