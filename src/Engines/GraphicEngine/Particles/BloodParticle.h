#ifndef BLOODPARTICLE_H
#define BLOODPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>

class BloodParticle: public ParticleManager{
public:	
	BloodParticle();
	~BloodParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif