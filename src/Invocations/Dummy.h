#ifndef DUMMY_H
#define DUMMY_H

#include "./Invocation.h"
#include <vector3d.h>

class GAnimation;

class Dummy: public Invocation{
public:
	Dummy(int HP, float time, bool alli, vector3df TPosition, vector3df TScale, vector3df TRotation);
	~Dummy();

	bool Update(float deltaTime);
	void SetName(std::string name);

private:
	void UpdatePosShape(float deltaTime);
	void CheckVelocity();
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);

	float 		currentTime; 		// Tiempo de duracion maxima del Dummy
	bool 		alliance; 			// De que alianza es? Mago/Brujo

	vector3df 	m_position;

	GAnimation* m_bottomBody;
};


#endif