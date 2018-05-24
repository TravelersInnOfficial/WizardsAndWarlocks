#ifndef FOUNTAINPARTICLE_H
#define FOUNTAINPARTICLE_H
#include <TOcularEngine/Elements/Particles/ParticleManager.h>
#include <vector3d.h>

class FountainParticle: public ParticleManager{
public:	
	FountainParticle();
	~FountainParticle();

	void InitParticle(Particle& p);
	void UpdateParticle(Particle& p, float deltaTime);

	void SetDirection(vector3df newDirection);
	
private:
	int velocity;
	vector3df direction;

};

#endif