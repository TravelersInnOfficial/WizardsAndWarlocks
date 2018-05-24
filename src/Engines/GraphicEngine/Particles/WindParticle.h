#ifndef WINDPARTICLE_H
#define WINDPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>
#include <vector3d.h>

class WindParticle: public ParticleManager{
public:	
	WindParticle();
	~WindParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);

	void SetDirection(vector3df newDirection);
	
private:
	int velocity;
	vector3df direction;

};

#endif