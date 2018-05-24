#ifndef EFFECTPARTICLE_H
#define EFFECTPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>

class EffectParticle: public ParticleManager{
public:	
	EffectParticle();
	~EffectParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif