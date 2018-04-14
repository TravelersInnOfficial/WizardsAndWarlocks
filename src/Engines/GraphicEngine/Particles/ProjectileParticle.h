#ifndef PROJECTILEPARTICLE_H
#define PROJECTILEPARTICLE_H
#include <TravelersOcularEngine/src/TOcularEngine/Elements/Particles/ParticleManager.h>

class ProjectileParticle: public ParticleManager{
public:	
	ProjectileParticle();
	~ProjectileParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);
	
private:

};

#endif