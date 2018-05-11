#ifndef FACE_H
#define FACE_H

#include "Align.h"

class Face: public Align{
public:
	Face();
	~Face();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
};


#endif