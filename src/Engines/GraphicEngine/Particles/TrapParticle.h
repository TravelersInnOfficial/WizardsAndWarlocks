#ifndef TRAPPARTICLE_H
#define TRAPPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>

class TrapParticle: public ParticleManager{
public:	
	TrapParticle();
	~TrapParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif