#ifndef FACE_H
#define FACE_H

#include <iostream>
#include <vector3d.h>
#include <vector2d.h>
#include <kinematicTypes.h>

#include "Align.h"

class Face: public Align{
public:
	Face();
	~Face();
	SteeringOutput GetSteering(Kinematic cKin, Kinematic tKin);
};


#endif