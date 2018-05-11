#ifndef BASET_H
#define BASET_H

#include "./Invocation.h"

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class BaseT: public Invocation{
public:
	BaseT(vector3df TPosition, vector3df TScale, vector3df TRotation);

	bool Update(float deltaTime);
	void Deactivate();
	vector3df GetPosition();

private:
	void CreateInvocation(vector3df TPosition, vector3df TScale, vector3df TRotation);
	bool active;
};


#endif